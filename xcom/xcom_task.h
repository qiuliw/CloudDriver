#pragma once
#include <functional>
#include <string>
#include "xcom.h"
#include "xtask.h"
#include "xmsg.h"

struct bufferevent;

enum Parse{
    Parse_None = 0,
    Parse_ING,
    Parse_OK,
    Parse_ERR,
};

// 待优化：使用可调用对象替代任务子类。将业务逻辑与任务逻辑解耦

// 对外暴露的socket任务类，连接接受与数据包接受处理函数
class XCOM_API XComTask : public XTask
{
public:
    XComTask(){};

    bool Init() override;
    virtual void ConnectedCB(); // 连接成功的消息回调，由业务类重载
    // 回调写
    virtual void WriteCB(); // 低水位回调，只会在缓冲区水位变化为低水位时触发一次。
    virtual void EventCB(short what);
    virtual void ReadCB(); // 解析消息得到数据包给业务函数处理
    virtual void Read(const XMsg* msg); // 具体的数据处理函数
    // 直接写
    virtual bool Write(const XMsg* msg); // 写入缓冲区
    virtual int Write(const char* data, int size); // 直接写入缓冲区

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
    Parse parse_ = Parse_None;


};