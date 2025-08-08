#include "widget.h"

#include <QApplication>

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
    // 初始化 socket库
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#else
    // 使用断开连接socket，管道断开，会发出断开信号，导致程序退出
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        return 1;
#endif

    QApplication a(argc, argv);
    Widget w;
    w.show();
    // 处理信号槽
    return a.exec();
}
