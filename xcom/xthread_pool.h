#pragma once

#include "xcom.h"
#include <mutex>
#include <vector>

class XThread;
class XTask;

class XCOM_API XThreadPool {
public:
    XThreadPool();
    ~XThreadPool();

    static XThreadPool* Get();
    // 创建好线程XThread对象
    bool Init(int threadCount = 10);
    // 分配任务到线程，使用基础轮询算法
    // 每个线程处理多个任务
    // 添加任务 XTask
    // 激活线程去处理
    void Dispatch(XTask *task);
    XThread *GetThread();
private:

    // 线程数量
    int thread_count_ = 0;
    // 轮询线程
    int last_thread_id_ = -1;
    // 线程池线程
    std::vector<XThread*> threads_;
    std::mutex mutex_; // 线程池队列锁
};




