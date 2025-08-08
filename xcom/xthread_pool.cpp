#include "xthread_pool.h"
#include "XThread.h"
#include <iostream>
XThreadPool::XThreadPool() {
}


XThreadPool::~XThreadPool() {
    // 清理资源
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto thread : threads_) {
        if(thread){
            delete thread;
        }
    }
}


XThreadPool* XThreadPool::Get()
{
    static XThreadPool pool;
    return &pool;    
}


// 初始化所有线程并启动线程
bool XThreadPool::Init(int threadCount)
{
    this->last_thread_id_ = -1;
    this->thread_count_ = threadCount;
    std::lock_guard<std::mutex> lock(mutex_);
    for(int i = 0; i < threadCount; i++)
    {
        XThread *t = new XThread();
        t->tId_ = i;
        threads_.push_back(t);
        t->Setup(); // 初始化通知管道
        t->Start(); // 启动线程
    }

    return true;
}

// 线程轮询
XThread* XThreadPool::GetThread()
{
    static int currentThreadId = 0;
    currentThreadId = currentThreadId++ % thread_count_;
    return threads_[currentThreadId++];
}

// 分发线程
void XThreadPool::Dispatch(XTask *task)
{
    std::cout << "XThreadPool::Dispatch" << std::endl;

    if (thread_count_ == 0){
        std::cout << "请先初始化线程池" << std::endl;
        return;
    }

    // 轮询
    if(!task) return;
    int tid = (last_thread_id_ + 1) % thread_count_;
    last_thread_id_ = tid;
    XThread* t = threads_[tid];

    t->AddTask(task);

    t->Notify(); // 激活线程
}

// 与muduo不同，这里的新线程不监听连接，只执行任务
