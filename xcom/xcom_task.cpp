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
    auto *task = (XComTask *)ctx;
    task->WriteCB();
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
    if (what & BEV_EVENT_CONNECTED){ // 连接成功
        cout << "BEV_EVENT_CONNECTED" << endl;
        bufferevent_write(bev_, "hello", 6);
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

void XComTask::ReadCB(){
    cout << "XComTask::ReadCB" << endl;
    bufferevent_read(bev_, read_buf_, sizeof read_buf_);
    cout << read_buf_ << endl;
}

void XComTask::WriteCB(){
    cout << "XComTask::WriteCB" << endl;
    bufferevent_write(bev_, write_buf_, strlen(write_buf_));
}
