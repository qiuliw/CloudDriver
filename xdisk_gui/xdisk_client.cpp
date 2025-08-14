#include "xdisk_client.h"
#include <iostream>
#include "xcom_task.h"
#include "xdir_task.h"
#include "xthread_pool.h"
#include "XUploadTask.h"
#include <iostream>

#include "Logger.h"
using namespace std;

bool XDiskClient::Init(){
    XThreadPool::Get()->Init(10);
    return true;
}

void XDiskClient::GetDir()
{
    // 1. 连接服务器，创建目录获取任务
    std::cout << "GetDir: " << server_ip_ << " " << server_port_ << " " << server_root_ << std::endl;
    auto task = new XDirTask();
    task->set_server_ip(server_ip_);
    task->set_server_port(server_port_);
    task->set_server_root(server_root_);
    // 2. 视图更新回调
    task->setDirCallback([this](std::string dirs)->void{
        LOG_INFO("client getDirq, emit SDir signal");
        SDir(dirs); // 视图更新信号
    });
    XThreadPool::Get()->Dispatch(task);
}

void XDiskClient::Upload(std::string path){
    LOG_INFO("client upload file path:", path);
    auto task = new XUploadTask();
    task->set_server_ip(server_ip_);
    task->set_server_port(server_port_);
    task->setFilePath(path);
    XThreadPool::Get()->Dispatch(task);
}
