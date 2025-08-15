
#pragma once
#include <fstream>
#include <xcom_task.h>

class XDownloadTask : public XComTask
{
public:
    void ConnectedCB() override;
    void Read(const XMsg* msg) override;
    void WriteCB() override;

    void setFileName(const std::string& name) { fileName_ = name; }
    void setSavePath(const std::string& path) { savePath_ = path; }
private:
    std::string fileName_;
    std::string savePath_;
    std::ofstream ofs_;
    int fileSize_ = 0;
    int recvPos_ = 0;
    bool readyRecv_ = false;
};
