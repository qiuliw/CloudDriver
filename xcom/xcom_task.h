#pragma once
#include <string>
#include "xcom.h"
#include "xtask.h"
#include "xmsg.h"

struct bufferevent;

// 对外暴露的socket任务类，连接接受与数据包接受处理函数
class XCOM_API XComTask : public XTask
{
public:
    XComTask(){};

    bool Init() override;
    virtual void EventCB(short what);
    void ReadCB(); // 读取数据包并调用数据处理函数
    virtual void Read(const XMsg* msg); // 具体的数据处理函数
    virtual bool Write(const XMsg* msg);

    std::string server_ip() { return server_ip_; }
    int server_port() { return server_port_; }
    void set_server_ip(std::string ip){ server_ip_ = ip; }
    void set_server_port(int port){ server_port_ = port; };

protected:
    char read_buf_[4096] = {0};
    char write_buf_[4096] = {0};

    struct bufferevent *bev_ = 0;
    std::string server_ip_ = ""; // 服务器IP
    int server_port_ = 0; // 服务器端口

    XMsg msg_;// 数据包缓存
};