#include "widget.h"

#include <iostream>

#include "./ui_widget.h"
#include "QMessageBox"
#include <qfiledialog.h>
#include <QString>
#include <QDebug>
#include <string>
#include "xdisk_client.h"

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 客户端初始化
    XDiskClient::Get()->Init();
    // 注册信号支持的类型
    qRegisterMetaType<std::string>("std::string");
    // 绑定目录获取信号
    connect(XDiskClient::Get(), &XDiskClient::SDir, this, &Widget::UpdateDir);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Refresh()
{
    // QMessageBox::information(this,"","Refresh");

    // 启用客户端
    string ip = ui->ipEdit->text().toStdString();
    string root = ui->pathEdit->text().toStdString();
    int port = ui->portBox->value();
    XDiskClient *client = XDiskClient::Get();
    client->set_server_ip(ip);
    client->set_server_port(port);
    client->set_server_root(root);


    // 1. 连接服务器

    // 服务器 IP 端口 路径

    // 2. 设置回调（NIO）
    client->GetDir();
}

void Widget::UpdateDir(std::string dirs){
    // "file1,1024;file2,4096;file3,10240"
    QString str = dirs.c_str();
    str = str.trimmed();
    if (str.isEmpty()) return;

    QStringList filestr = str.split(';');
    // 删除所有行
    while (ui->fileListWidget->rowCount() > 0) {
        ui->fileListWidget->removeRow(0);
    }
    // ui->fileListWidget->clear(); // 只删除内容？
    for (int i = 0; i < filestr.size(); i++){
        QStringList fileinfo = filestr[i].split(',');
        if (fileinfo.size() >= ui->fileListWidget->columnCount()){
            // 插入到列表
            ui->fileListWidget->insertRow(0); // 插入在开头位置
            ui->fileListWidget->setItem(0, 0, new QTableWidgetItem(fileinfo[0]));
            ui->fileListWidget->setItem(0, 1, new QTableWidgetItem(tr("%1Byte").arg(fileinfo[1])));
        }

    }

}

void Widget::Upload()
{
    QMessageBox::information(this,"","Upload");
    // 用户选择一个文件
    QString filename = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("请选择上传文件"));
    if(filename.isEmpty())
        return;

    // 插入到列表
    ui->fileListWidget->insertRow(0); // 插入在开头位置

    // 获取文件大小（字节） - 使用QFileInfo（推荐，无需打开文件）
    QFileInfo fileInfo(filename);
    qint64 fileSize = fileInfo.size(); // 直接获取文件大小（字节）

    // 设置文件名和大小到列表
    ui->fileListWidget->setItem(0, 0, new QTableWidgetItem(fileInfo.fileName())); // 文件名
    ui->fileListWidget->setItem(0, 1, new QTableWidgetItem(tr("%1 Byte").arg(fileSize))); // 文件大小

}
