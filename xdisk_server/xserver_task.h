#pragma once
#include "xtask.h"

typedef void (*ListenCBFunc)(int client_socket, struct sockaddr *sockaddr, int socklen, void *arg);

class XServerTask : public XTask
{
public:
    bool Init() override;
    ListenCBFunc listen_cb_ = 0;
    void set_server_port(int port) {server_port_ = port;}
private:
    int server_port_ = 0;
};
