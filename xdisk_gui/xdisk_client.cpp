#include "xdisk_client.h"
#include <iostream>


void XDiskClient::GetDir()
{
    std::cout << "GetDir:" << server_ip_ << " " << server_port_ << " " << server_root_ << std::endl;
}
