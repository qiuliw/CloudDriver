#pragma once

// 任务基类 分发到线程池后Init被回调，base由线程池注入
class XTask {
public:
    // 初始化任务
    virtual bool Init() = 0; // ReadNotify 线程读取消息并执行回调中执行

    struct event_base *base() { return base_; }
    void set_base(struct event_base * base) { base_ = base; }
    int sock(){return sock_;}
    void set_sock(int sock){sock_ = sock;}

private:
    struct event_base *base_ = 0;
    int sock_ = 0; // 新连接回调时候保存
    int threadId_ = 0; // XThread::AddTask时候保存

};