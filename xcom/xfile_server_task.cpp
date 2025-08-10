//
// Created by 18071 on 2025/8/8.
//

#include "xfile_server_task.h"

#include <iostream>

#include "xtools.h"

using namespace std;

void XFileServerTask::Read(const XMsg* msg){
    switch (msg->type){
        case MSG_GETDIR:
            cout << "MSG_GETDIR" << endl;
            GetDir(msg);
            break;
        default:
            break;
    }
}

// 处理目录获取的消息，返回目录列表
void XFileServerTask::GetDir(const XMsg* msg){
    if (!msg->data) return;
    string path = msg->data;
    if (path.empty()){
        path = "./";
    }
    // string dir = "file1,1024;file2,4096;file3.zip,10240";
    string dir = GetDirData(path);

    XMsg resmsg;
    resmsg.type = MSG_DIRLIST;
    resmsg.data = (char *)dir.c_str();
    resmsg.size = dir.size() + 1;
    Write(&resmsg);
}
