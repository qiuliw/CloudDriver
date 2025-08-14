//
// Created by 18071 on 2025/8/8.
//

#include "xfile_server_task.h"

#include <corecrt_io.h>
#include <iostream>
#include <event2/bufferevent.h>

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
    switch (msg->type){
        case MSG_GETDIR:
            cout << "MSG_GETDIR" << endl;
            GetDir(msg);
            break;
        case MSG_UPLOAD:
            UploadReady(msg);
        default:
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

    XMsg resmsg;
    resmsg.type = MSG_DIRLIST;
    resmsg.data = (char *)dir.c_str();
    resmsg.size = dir.size() + 1;
    Write(&resmsg);
}

void XFileServerTask::UploadReady(const XMsg* msg){
    if (!msg->data || !msg || msg->size <= 0) return;
    string str = msg->data;
    if (str.empty()) return;
    int pos = str.find_last_of(",");
    if (pos <= 0) return;
    string filename = str.substr(0, pos);
    if (pos + 1 >= str.size()) return;
    string tmp = str.substr(pos + 1, str.size() - pos - 1);
    fileSize_ = atoi(tmp.c_str());
    cout << "filename:" << filename << " size:" << tmp << endl;
    string filepath = serverPath_ + filename;
    ofs_.open(filepath);
    if (!ofs_.is_open()){
        LOG_WARN("server openfile err:%s",filepath.c_str());
        return;
    }


    WriteStr(MSG_UPLOAD_ACCEPT, "OK");
    isRecv_ = true;
}

// 当包消息接收完毕时调用
void XFileServerTask::RecvFile(){
    if (!ofs_.is_open()){
        LOG_ERROR("ofs not open, RecvFile fatal");
        return;
    }
    int readsize = fileSize_ - fileRecved_;
    readsize = sizeof (read_buf_) < readsize ? sizeof (read_buf_) : readsize;
    int len = bufferevent_read(bev_, read_buf_ ,readsize);
    if (len <= 0){
        LOG_ERROR("bufferevent_read err");
        return;
    }
    fileRecved_ += len;
    if (fileRecved_ == fileSize_){
        parse_ = Parse_None;
        isRecv_ = false;
        WriteStr(MSG_UPLOAD_COMPLETE, "OK");
        ofs_.close();
        LOG_INFO("file recv complete");
    }
}

void XFileServerTask::WriteStr(MsgType type, const std::string& str){
    XMsg msg;
    msg.type = type;
    msg.data = (char *)str.c_str();
    msg.size = str.size() + 1;
    Write(&msg);
}
