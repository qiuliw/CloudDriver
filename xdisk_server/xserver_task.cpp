#include "xserver_task.h"

#include <event2/event.h>
#include <event2/listener.h>
#include <iostream>
#include <cstring>
#include "xfile_server_task.h"


// 新连接回调
static void SListenCB(struct evconnlistener * evc, evutil_socket_t client_socket, struct sockaddr *sockaddr, int socklen, void *arg)
{
    std::cout << "XServerTask::SListenCB" << std::endl;
    XServerTask *task = (XServerTask *)arg;
    if (task->listen_cb_)
        task->listen_cb_(client_socket,sockaddr,socklen,arg);
    else{
        std::cerr << "please set callback function ListenCB" << std::endl;
    }
}

bool XServerTask::Init()
{
    if(server_port_ <= 0){
        std::cerr << "XServerTask::Init failed, server_port_ is not set" << std::endl;
        return false;
    }

    // 监听端口
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(server_port_);
    // 设置回调函数
    auto evc = evconnlistener_new_bind(
        base(), // 线程池提供base_ 每个线程池提供一个base_
        SListenCB, 
        this, 
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 
        10, 
        (sockaddr *) &sin,
        sizeof(sin)
    );
    if (!evc) {
        std::cout << "listen port" << server_port_ << " failed" << std::endl;
        return false;
    }

    std::cout << "listen port" << server_port_ << " success" << std::endl;

    return true;
}
