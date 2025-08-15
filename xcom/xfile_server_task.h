#pragma once

#include <fstream>

#include "Logger.h"
#include "xcom.h"
#include "xcom_task.h"

class XCOM_API XFileServerTask : public XComTask{
public:

    void ReadCB() override;

    void Read(const XMsg* msg) override;



private:
    // 处理目录获取的消息，返回目录列表
    void GetDir(const XMsg* msg);

    // 处理客户端的上传请求
    void UploadReady(const XMsg *msg);
    // 收到消息包后，再读取消息就是纯数据
    void RecvFile();

    // 处理客户端的下载请求
    void Download(const XMsg *msg);

    void WriteStr(MsgType, const std::string&);

    std::string serverPath_ = "./";
    std::string filePath_ = "";

    bool isRecv_ = false;
    int fileSize_ = 0;
    int fileRecved_ = 0;
    std::string fileName_;

    std::ifstream ifs_;
    std::ofstream ofs_;

};



