//
// Created by 18071 on 2025/8/13.
//

#ifndef XUPLOADTASK_H
#define XUPLOADTASK_H

#include <fstream>
#include <xcom_task.h>

class XUploadTask : public XComTask
{
public:
    // 连接成功的消息回调
    void ConnectedCB() override;

    // 读取消息的回调
    virtual void Read(const XMsg* msg) override;

    void setFilePath(std::string path){
        filePath_ = path;
    }
private:
    // 需要上传的文件路径
    std::string filePath_ = "";
    int fileSize_ = 0;

    // 读取文件
    std::ifstream ifs_;
};

#endif //XUPLOADTASK_H
