#pragma once

#include "xcom.h"

#define MSG_MAX_SIZE 1000000 // 1M 消息最大字节数

enum MsgType {
    MSG_NONE = 0,
    MSG_GETDIR, // 请求目录
    MSG_DIRLIST, // 返回目录列表
    MSG_UPLOAD, // 请求上传文件
    MSG_UPLOAD_ACCEPT, // 服务端准备好接受文件,
    MSG_UPLOAD_COMPLETE, // 服务端接受文件结束
    MSG_DOWNLOAD, // 请求下载文件
    MSG_DOWNLOAD_ACCEPT, // 服务端准备好发送文件
    MSG_DOWNLOAD_COMPLETE, // 服务端发送文件结束
    MSG_ERROR, // 错误消息类型
    MSG_MAX_TYPE, // 用来验证类型正确，枚举最大值
};

// 结构体作为函数参数或返回值时，是函数在动态库中签名的一部分，同样需要导入导出，加载到符号表中
// 消息头
struct XCOM_API XMsgHead{
    MsgType type; // 消息类型
    int size; // 消息长度
};
// 消息体
struct XCOM_API XMsg : public XMsgHead {
    char *data = 0; // 存储消息内容
    int recved = 0; // 已接收的字节数
};