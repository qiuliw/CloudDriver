#pragma once

#include <string>
class XDiskClient
{
public:
    // 单例
    static XDiskClient* Get()
    {
        static XDiskClient xc;
        return &xc;
    }
    // 初始化，包括线程池
    bool Init();

    /**
     * @brief 获取目录下的文件列表，只是请求消息到服务端。非阻塞，在回调中处理结果。
     * 
     * @param path 目录路径
     */
    void GetDir();
    
    void set_server_ip(std::string& ip) { server_ip_ = ip; }
    void set_server_port(int port) { server_port_ = port; }
    void set_server_root(std::string& root) { server_root_ = root; }
private:
    XDiskClient(){};

    std::string server_ip_ = ""; // 服务器IP
    int server_port_ = 0; // 服务器端口
    std::string server_root_ = ""; // 服务器根目录 

};