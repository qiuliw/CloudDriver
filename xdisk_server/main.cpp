#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif

#include <xthread_pool.h>

int main(int argc, char *argv[] ){

#ifdef _WIN32
    // 初始化 socket库
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#else
    // 使用断开连接socket，管道断开，会发出断开信号，导致程序退出
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        return 1;
#endif

    int server_port = 20010;
    int thread_count = 10;
    if(argc > 1)
        server_port = atoi(argv[1]);
    if(argc > 2)
        thread_count = atoi(argv[2]);
    if(argc == 1)
        std::cout << "Usage: " << argv[0] << " [server_port] [thread_count]" << std::endl;

    XThreadPool::Get()->Init(thread_count);
    

}