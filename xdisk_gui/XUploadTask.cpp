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
    // 发送上传文件请求
    XMsg msg;
    msg.type = MSG_UPLOAD;
    msg.size = strlen(data) + 1;
    msg.data = data;
    Write(&msg);
}

void XUploadTask::Read(const XMsg* msg){
    XComTask::Read(msg);
    cout << "XUploadTask::Read" << endl;
    cout << "msg->data" << msg->data << endl;
}
