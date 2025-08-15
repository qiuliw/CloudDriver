
#include "XDownloadTask.h"
#include "Logger.h"
#include <QMessageBox>
#include <QString>
#include <cstring>

using namespace std;

void XDownloadTask::ConnectedCB() {
    LOG_INFO("DownloadTask ConnectedCB, fileName: %s", fileName_.c_str());
    // 发送下载请求：文件名
    char data[1024] = {0};
    snprintf(data, sizeof(data), "%s", fileName_.c_str());
    WriteMessage(MSG_DOWNLOAD, data);
}

void XDownloadTask::Read(const XMsg* msg) {
    LOG_INFO("DownloadTask Read, type: %d", msg->type);
    if (msg->type == MSG_ERROR) {
        // 错误消息，弹窗提示
        QMessageBox::critical(nullptr, "下载错误", QString::fromLocal8Bit(msg->data));
        readyRecv_ = false;
        if (ofs_.is_open()) ofs_.close();
        return;
    }
    if (msg->type == MSG_DOWNLOAD_ACCEPT) {
        // 解析文件大小
        fileSize_ = atoi(msg->data);
        LOG_INFO("Server accepted download, fileSize: %d", fileSize_);
        ofs_.open(savePath_, ios::out | ios::binary);
        if (!ofs_.is_open()) {
            LOG_ERROR("Failed to open save file: %s", savePath_.c_str());
            return;
        }
        readyRecv_ = true;
    } else if (msg->type == MSG_DOWNLOAD_COMPLETE) {
        readyRecv_ = false;
        if (ofs_.is_open()) ofs_.close();
        LOG_INFO("Download complete");
    } else if (readyRecv_ && msg->type == MSG_NONE) {
        // 默认数据包，写入文件
        ofs_.write(msg->data, msg->size);
        recvPos_ += msg->size;
        LOG_INFO("Received %d bytes, total: %d/%d", msg->size, recvPos_, fileSize_);
        if (recvPos_ >= fileSize_) {
            readyRecv_ = false;
            if (ofs_.is_open()) ofs_.close();
            LOG_INFO("File download finished");
        }
    }
}

void XDownloadTask::WriteCB() {
    // 下载一般无需写回调
}
