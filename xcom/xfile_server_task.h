#pragma once

#include "xcom.h"
#include "xcom_task.h"

class XCOM_API XFileServerTask : public XComTask{
public:
    void Read(const XMsg* msg) override;
private:
    // 处理目录获取的消息，返回目录列表
    void GetDir(const XMsg* msg);
};



