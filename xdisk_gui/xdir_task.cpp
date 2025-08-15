//
// Created by 18071 on 2025/8/9.
//

#include "xdir_task.h"

#include <iostream>
#include <cstring>

using namespace std;

void XDirTask::Read(const XMsg* msg){
    // 接受到服务端发送的目录
    switch (msg->type){
        case MSG_DIRLIST: // 服务器返回的目录列表
            cout << "MSG_DIRLIST" << endl;
            if (dirCallback_)
                dirCallback_(msg->data);
            break;
    }
}
// 连接建立回调
void XDirTask::ConnectedCB(){
    cout << "ConnectedCB" << server_root_.size() << server_root_ << endl;
    
    bool success = WriteMessage(MSG_GETDIR, server_root_);
    if (!success) {
        cout << "Write failed for MSG_GETDIR" << endl;
    }
}