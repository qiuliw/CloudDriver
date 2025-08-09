#pragma once
#include "xcom_task.h"
#include "xtask.h"

typedef void (*ListenCBFunc)(int client_socket, struct sockaddr *sockaddr, int socklen, void *arg);

class XServerTask : public XComTask
{
public:
    bool Init() override;
    ListenCBFunc listen_cb_ = 0;
private:
};
