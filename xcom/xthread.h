#pragma once
#include <event2/event.h>
#include <list>
#include <thread>
#include <mutex>

#include "xtask.h"
#include "xcom.h"

class XThread {
public:
    XThread();
    ~XThread();

    void Start();
    void Main();
    bool Setup();
    bool Notify(); // 激活线程
    bool ReadNotify(); // 读取消息
    bool Run(); // 接受消息并执行任务

    // 添加处理的任务，一个线程同时处理多个任务，共用一个event_base
    void AddTask(XTask *task);

    int tId_ = 0; // 线程ID

private:
    
    bool InitNotifier(); // 初始化管道
    // 注册监听消息管道
    bool ListenNotifier(event_base* base, event_callback_fn cb, void* arg);

    struct event_base* base_ = nullptr;

    // 线程间通信管道
#ifdef _WIN32
    evutil_socket_t notifyFds_[2] = {0, 0}; // win平台使用socket线程间通信
#else
    int efd_ = -1; // linux平台使用eventfd线程间通信
#endif
    
    std::list<XTask*> tasks_; // 任务列表
    std::mutex tasksMutex_; // 任务列表的锁
};