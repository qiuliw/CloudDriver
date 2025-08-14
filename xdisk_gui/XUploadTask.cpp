//
// Created by 18071 on 2025/8/13.
//

#include "XUploadTask.h"
#include <iostream>

#include "Logger.h"

using namespace std;

// 发送发文件请求
void XUploadTask::ConnectedCB(){
    if (filePath_.empty()){
        LOG_WARN("filePath is empty");
        return;
    }
    // 跳转到结尾来获取文件大小
    ifs_.open(filePath_.c_str(),ios::in|ios::binary|ios::ate);
    if (!ifs_.is_open()){
        return;
    }
    // 获取文件大小 tellg 返回当前文件读取指针的位置（类型为 std::streampos），通常表示从文件开头到指针位置的字节数。若文件为空或未打开，可能返回 -1
    fileSize_ = ifs_.tellg();
    ifs_.seekg(0,ios::beg);
    cout << "open file " << filePath_ << " size " << fileSize_ << endl;
    string filename = "";
    int pos = filePath_.find_last_of("/");
    if (pos < 0)pos = 0;
    filename = filePath_.substr(pos+1, filePath_.size() - pos - 1);
    cout << "filename " << filename << endl;

    char data[1024] = {0};
    sprintf(data,"%s,%d",filename.c_str(),fileSize_);
    WriteStr(MSG_UPLOAD,data);
}

void XUploadTask::Read(const XMsg* msg){
    if (msg->type == MSG_UPLOAD_ACCEPT){
        sendFile();
        readySend = true;
    }
    if (msg->type == MSG_UPLOAD_COMPLETE){
        readySend = false;
        LOG_INFO("upload complete");
    }

}


void XUploadTask::WriteCB(){
    if (readySend){
        sendFile();
    }
}

void XUploadTask::sendFile(){

    if (!ifs_.is_open()  || fileSize_ <= 0){
        LOG_ERROR("ifs err");
        return;
    }
    // 定位到文件的当前发送位置
    ifs_.seekg(sendPos_, ios::beg);
    if (ifs_.fail()) {
        LOG_ERROR("Failed to seek to position: %d", sendPos_);
        return;
    }
    char buf[1024] = {0};
    // 从文件流中读取数据
    ifs_.read(buf, sizeof(buf));
    std::streamsize bytesRead = ifs_.gcount();

    if (bytesRead == 0){
        LOG_INFO("ifs not data");
        readySend = false;
        ifs_.close();
    }

    // 如果读取到数据，则发送给服务器
    if (bytesRead > 0) {
        // 直接使用bufferevent发送数据
        int len = Write(buf, bytesRead);
        if (len < 0) {
            LOG_ERROR("bufferevent_write failed");
        }
        sendPos_ = sendPos_ + bytesRead;

    }
    // 检查是否已经读取完所有数据
    if (ifs_.eof() || ifs_.tellg() >= fileSize_) {
        // 文件发送完毕
        readySend = false;
        ifs_.close();
        LOG_INFO("File upload completed");
    }
}


void XUploadTask::WriteStr(MsgType type, const std::string& str){
    XMsg msg;
    msg.type = type;
    msg.data = (char *)str.c_str();
    msg.size = str.size() + 1;
    Write(&msg);
}
