//
// Created by 18071 on 2025/8/8.
//

#include "xfile_server_task.h"

#include <iostream>
#include <event2/bufferevent.h>
#include <cstring>

#include "Logger.h"
#include "xtools.h"
using namespace std;

void XFileServerTask::ReadCB(){
    if (parse_ == Parse_None){
        cout << "解析消息包" << endl;
        XComTask::ReadCB();
    }
    else if (parse_ == Parse_OK && isRecv_) {
        RecvFile();
    }
}

void XFileServerTask::Read(const XMsg* msg){
    LOG_INFO("Server received message type: %d", msg->type);
    switch (msg->type){
        case MSG_GETDIR:
            cout << "MSG_GETDIR" << endl;
            GetDir(msg);
            break;
        case MSG_UPLOAD:
            LOG_INFO("Processing MSG_UPLOAD");
            UploadReady(msg);
            break;
        default:
            LOG_WARN("Unknown message type: %d", msg->type);
            break;
    }
}



// 处理目录获取的消息，返回目录列表
void XFileServerTask::GetDir(const XMsg* msg){
    if (!msg->data) return;
    serverPath_ = msg->data;
    if (serverPath_.empty()){
        serverPath_ = "./";
    }
    // string dir = "file1,1024;file2,4096;file3.zip,10240";
    string dir = GetDirData(serverPath_);

    LOG_INFO("Sending directory list: %s", dir.c_str());
    bool success = WriteMessage(MSG_DIRLIST, dir);
    if (!success) {
        LOG_ERROR("Write failed for MSG_DIRLIST");
    }
}

void XFileServerTask::UploadReady(const XMsg* msg){
    LOG_INFO("UploadReady called");
    if (!msg->data || !msg || msg->size <= 0) {
        LOG_ERROR("Invalid message in UploadReady");
        return;
    }
    string str = msg->data;
    if (str.empty()) {
        LOG_ERROR("Empty message data in UploadReady");
        return;
    }
    LOG_INFO("Upload request data: %s", str.c_str());
    
    int pos = str.find_last_of(",");
    if (pos <= 0) {
        LOG_ERROR("Invalid format in upload request, no comma found");
        return;
    }
    string filename = str.substr(0, pos);
    if (pos + 1 >= str.size()) {
        LOG_ERROR("Invalid format in upload request, no size after comma");
        return;
    }
    string tmp = str.substr(pos + 1, str.size() - pos - 1);
    fileSize_ = atoi(tmp.c_str());
    LOG_INFO("Parsed filename: %s, size: %d", filename.c_str(), fileSize_);
    
    string filepath = serverPath_;
    if (!filepath.empty()){
        char back = filepath.back();
        if (back != '/' && back != '\\') filepath += "/";
    }
    filepath += filename;
    LOG_INFO("Full file path: %s", filepath.c_str());
    
    // open file in binary mode and truncate existing
    ofs_.open(filepath, ios::out | ios::binary | ios::trunc);
    if (!ofs_.is_open()){
        LOG_WARN("server openfile err:%s",filepath.c_str());
        return;
    }
    LOG_INFO("File opened successfully for writing");

    WriteStr(MSG_UPLOAD_ACCEPT, "OK");
    isRecv_ = true;
    fileRecved_ = 0;
    LOG_INFO("Upload ready, waiting for file data");
}

// 当包消息接收完毕时调用
void XFileServerTask::RecvFile(){
    if (!ofs_.is_open()){
        LOG_ERROR("ofs not open, RecvFile fatal");
        return;
    }
    int readsize = fileSize_ - fileRecved_;
    readsize = sizeof (read_buf_) < readsize ? sizeof (read_buf_) : readsize;
    LOG_INFO("Attempting to read %d bytes, fileRecved: %d, fileSize: %zd", readsize, fileRecved_, fileSize_);
    
    int len = bufferevent_read(bev_, read_buf_ ,readsize);
    if (len <= 0){
        LOG_ERROR("bufferevent_read err");
        return;
    }
    LOG_INFO("Read %d bytes from network", len);
    
    // write chunk to file
    ofs_.write(read_buf_, len);
    if (!ofs_){
        LOG_ERROR("write file failed");
        return;
    }
    fileRecved_ += len;
    LOG_INFO("Written %d bytes to file, total received: %d/%d", len, fileRecved_, fileSize_);
    
    if (fileRecved_ == fileSize_){
        parse_ = Parse_None;
        isRecv_ = false;
        WriteStr(MSG_UPLOAD_COMPLETE, "OK");
        ofs_.close();
        LOG_INFO("file recv complete");
    }
}

void XFileServerTask::WriteStr(MsgType type, const std::string& str){
    LOG_INFO("Server WriteStr: type=%d, data=%s, size=%d", type, str.c_str(), str.size() + 1);
    
    bool success = WriteMessage(type, str);
    if (!success) {
        LOG_ERROR("Server Write failed for message type: %d", type);
    }
}
