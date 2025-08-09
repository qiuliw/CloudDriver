#pragma once
#include <string>
#include "xcom.h"
#include "xtask.h"

struct bufferevent;

// 对外暴露的服务器任务类，连接接受与处理函数
class XCOM_API XComTask : public XTask
{
public:
    XComTask(){};

    bool Init() override;
    void EventCB(short what);
    void ReadCB();
    void WriteCB();

    std::string server_ip() { return server_ip_; }
    int server_port() { return server_port_; }
    void set_server_ip(std::string ip){ server_ip_ = ip; }
    void set_server_port(int port){ server_port_ = port; };

protected:
    char read_buf_[4096] = {0};
    char write_buf_[4096] = {0};

private:
    struct bufferevent *bev_ = 0;
    std::string server_ip_ = "";
    int server_port_ = 0;
};