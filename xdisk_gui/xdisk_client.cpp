#include "xdisk_client.h"
#include <iostream>
#include "xcom_task.h"
#include "xdir_task.h"
#include "xthread_pool.h"

using namespace std;

bool XDiskClient::Init(){
    XThreadPool::Get()->Init(10);
    return true;
}

void XDiskClient::GetDir()
{
    std::cout << "GetDir:" << server_ip_ << " " << server_port_ << " " << server_root_ << std::endl;
    auto task = new XDirTask();
    task->set_server_ip(server_ip_);
    task->set_server_port(server_port_);
    task->setDirCallback([](std::string dirs)->void{
        cout << "dirs:" << dirs << endl;
    });
    // 现在不能操作，task未初始化。task没有event_base
    XThreadPool::Get()->Dispatch(task);

}
