#include "xcom_task.h"

#include <event2/bufferevent.h>
#include <iostream>
#include <event2/event.h>
using namespace std;

// 由静态函数中转为成员函数
static void SReadCB(struct bufferevent *bev, void *ctx){
    auto *task = (XComTask *)ctx;
    task->ReadCB();
}
static void SWriteCB(struct bufferevent *bev, void *ctx){

}

static void SEventCB(struct bufferevent *bev, short what, void *ctx){
    auto *task = (XComTask *)ctx;
    task->EventCB(what);
}

// 通用sock任务类：客户端任务类（设置server_ip则主动连接服务器并创建新sock）与服务端任务类（有sock则使用这个sock接受连接）
bool XComTask::Init()
{
    // 区分服务端还是客户端：服务端任务会先绑定listen socket后传入事件循环，客户端任务不会
    int comsock = this->sock();
    if (comsock <= 0)
        comsock = -1;
    // 用bufferevent建立连接

    // 创建bufferevent上下文，-1表示创建新的socket
    bev_ = bufferevent_socket_new(base(),comsock,BEV_OPT_CLOSE_ON_FREE);
    if(!bev_){
        cerr << "bufferevent_socket_new error" << endl;
        return false;
    }
    // 设置回调函数
    bufferevent_setcb(bev_, SReadCB, SWriteCB, SEventCB, this);
    bufferevent_enable(bev_, EV_READ | EV_WRITE);
    timeval tv = {10,0};
    bufferevent_set_timeouts(bev_, &tv, &tv);

    // 连接服务器
    if (server_ip_.empty()){
        return true;
    }
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(server_port_);
    evutil_inet_pton(AF_INET,server_ip_.c_str(), &sin.sin_addr); // 跨平台的IP地址转换
    // 非阻塞的由事件循环管理的connect,连接结果通过回调函数异步通知
    int re = bufferevent_socket_connect(bev_, (struct sockaddr *)&sin, sizeof(sin));
    if (re != 0)
            return false;

    return true;
}

// 退出要处理缓冲内容
void XComTask::EventCB(short what){
    cout << "XComTask::EventCB" << endl;
    if (what & BEV_EVENT_CONNECTED){ // 连接成功，客户端主动建立连接时触发
        ConnectedCB();
    }
    if (what & BEV_EVENT_ERROR || what & BEV_EVENT_TIMEOUT){ // 错误
        cout << "BEV_EVENT_ERROR or BEV_EVENT_TIMEOUT" << endl;
        bufferevent_free(bev_);
    }
    if (what & BEV_EVENT_EOF){ // 对方关闭连接
        cout << "BEV_EVENT_EOF" << endl;
        bufferevent_free(bev_);
    }
}

// libevent默认水平触发不用关心粘包，也可以外层加for(;;)阻塞读取
void XComTask::ReadCB(){
    // 接受消息 XMsgHead

    // 1.接受头部信息
    if (!msg_.data){
        int headsize = sizeof(XMsgHead);
        int len = bufferevent_read(bev_, &msg_, headsize);
        if (len != headsize){
            cerr << "msg head recv error" << endl;
            return;
        }
        // 验证消息头有效性
        if (msg_.type >= MSG_MAX_TYPE || msg_.size < 0 || msg_.size > MSG_MAX_SIZE){
            cerr << "msg head is error" << endl;
            return;
        }
        msg_.data = new char[msg_.size]; // 连接突然断，也要释放内存
    }
    // 2.接受数据
    int readsize = msg_.size - msg_.recved;
    if (readsize <= 0){ //格式出错。 =0 表示接受完毕后还被触发，则直接返回
        if (msg_.data)
            delete msg_.data;
        memset(&msg_, 0, sizeof(msg_));

    }
    int len = bufferevent_read(bev_, msg_.data + msg_.recved, msg_.size);
    if (len <= 0){
        return;
    }
    msg_.recved += len;
    if (msg_.recved == msg_.size){
        cout << "recv msg end" << endl;
        Read(&msg_);
        if (msg_.data)
            delete msg_.data;
        memset(&msg_, 0, sizeof(msg_));
    }

}

// 连接成功回调
void XComTask::ConnectedCB(){
    XMsg msg;
    msg.type = MSG_DIRLIST;
    msg.data = (char*) "OK";
    msg.size = strlen(msg.data) + 1;
    Write(&msg);
}

bool XComTask::Write(const XMsg* msg){
    if (!bev_ || !msg || !msg->data || msg->size <= 0)
        return false;
    // 1.写入消息头
    int re = bufferevent_write(bev_, msg, sizeof(XMsgHead));
    if (re != 0) return false;
    // 2.写入数据内容
    re = bufferevent_write(bev_, msg->data, msg->size);
    return true;
}


// 接受消息的回调，由业务重载
void XComTask::Read(const XMsg* msg){
    cout << "msg->size: " << msg->size << endl;
    cout << "msg->type: ";
    switch (msg->type){
        case MSG_GETDIR:
            cout << "MSG_GETDIR" << endl;
            break;
        case MSG_DIRLIST:
            cout << "MSG_DIRLIST" << endl;
            break;
        default:
            cout << "other type " << msg_.type << endl;
    }
    cout << "msg->data: " << msg->data << endl;
}
