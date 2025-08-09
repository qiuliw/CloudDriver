#pragma once

#include <functional>
#include <string>

#include <xcom_task.h>

// typedef void (*DirCBFunc)(std::string dirs);
// using DirCallback = void(*)(std::string dirs);
using DirCallback = std::function<void(std::string dirs)>;

// 可以使用可调用对象代替，与业务紧密相关的，使用可调用对象传入回调，在业务代码中实现。
class XDirTask :public XComTask{
public:
    void Read(const XMsg* msg) override;
    void ConnectedCB() override;
    void setDirCallback(DirCallback cb){
        dirCallback_ = std::move(cb);
    }
private:
    // 获取目录后调用的回调
    DirCallback dirCallback_;
};



