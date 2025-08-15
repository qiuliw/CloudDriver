#include "xdisk_client.h"
#include <iostream>
#include "xdir_task.h"
#include "xthread_pool.h"
#include "XUploadTask.h"
#include "XDownloadTask.h"
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
    // 2. 视图更新回调 - 使用lambda但不捕获this
    task->setDirCallback([](std::string dirs)->void{
        LOG_INFO("client getDir, emit SDir signal");
        // 使用全局函数或静态函数来发送信号
        XDiskClient::Get()->SDir(dirs);
    });
    XThreadPool::Get()->Dispatch(task);
}

void XDiskClient::Upload(std::string path){
    LOG_INFO("client upload file path: %s", path.c_str());
    auto task = new XUploadTask();
    task->set_server_ip(server_ip_);
    task->set_server_port(server_port_);
    task->setFilePath(path);
    XThreadPool::Get()->Dispatch(task);
}

void XDiskClient::Download(const std::string& filename, const std::string& savePath) {
    LOG_INFO("client download file: %s to %s", filename.c_str(), savePath.c_str());
    auto task = new XDownloadTask();
    task->set_server_ip(server_ip_);
    task->set_server_port(server_port_);
    task->setFileName(filename);
    task->setSavePath(savePath);
    XThreadPool::Get()->Dispatch(task);
}
