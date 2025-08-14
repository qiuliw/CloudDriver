#pragma once

#include "xcom.h"
#include "xcom_task.h"

class XCOM_API XFileServerTask : public XComTask{
public:

    void ReadCB() override;

    void Read(const XMsg* msg) override;

    void ReadFile();

private:
    // 处理目录获取的消息，返回目录列表
    void GetDir(const XMsg* msg);
    // 处理客户端的上传请求
    void Upload(const XMsg *msg);



    int FileSize = 0;
};



