//
// Created by 18071 on 2025/8/9.
//

#include "xdir_task.h"

#include <iostream>

using namespace std;

void XDirTask::Read(const XMsg* msg){
    // 接受到服务端发送的目录
    switch (msg->type){
        case MSG_DIRLIST: // 服务器返回的目录列表
            if (dirCallback_){
                dirCallback_(msg->data);
            }
            break;
    }
}
// 连接建立回调
void XDirTask::ConnectedCB(){
    XMsg msg;
    msg.type = MSG_GETDIR;
    msg.size = 3;
    msg.data = (char *)"./";
    Write(&msg);
}