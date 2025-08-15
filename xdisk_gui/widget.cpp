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

void Widget::UpdateServerInfo(){
    // 同步 View数据到Controller
    string ip = ui->ipEdit->text().toStdString();
    string root = ui->pathEdit->text().toStdString();
    int port = ui->portBox->value();
    XDiskClient::Get()->set_server_ip(ip);
    XDiskClient::Get()->set_server_port(port);
    XDiskClient::Get()->set_server_root(root);
}

void Widget::Refresh()
{
    UpdateServerInfo();
    XDiskClient::Get()->GetDir();
}

void Widget::UpdateDir(std::string dirs){
    // "file1,1024;file2,4096;file3,10240" 或服务端错误消息
    QString str = dirs.c_str();
    str = str.trimmed();
    if (str.startsWith("ERROR") || str.startsWith("error") || str == "" || !str.contains(",")) {
        // 显示错误提示
        QMessageBox::critical(this, "刷新失败", str.isEmpty() ? "路径错误或服务器无响应" : str);
        // 清空表格
        while (ui->fileListWidget->rowCount() > 0) {
            ui->fileListWidget->removeRow(0);
        }
        return;
    }

    QStringList filestr = str.split(';');
    // 删除所有行
    while (ui->fileListWidget->rowCount() > 0) {
        ui->fileListWidget->removeRow(0);
    }
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
    // QMessageBox::information(this,"","Upload");
    // 用户选择一个文件
    QString filename = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("请选择上传文件"));
    if(filename.isEmpty())
        return;

    UpdateServerInfo();
    XDiskClient::Get()->Upload(filename.toUtf8().toStdString());

}

void Widget::Download()
{
    // 用户选择要下载的文件
    int row = ui->fileListWidget->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "提示", "请先选择要下载的文件");
        return;
    }
    QString filename = ui->fileListWidget->item(row, 0)->text();
    if(filename.isEmpty()) {
        QMessageBox::warning(this, "提示", "文件名为空");
        return;
    }
    // 选择保存路径
    QString savePath = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请选择保存位置"), filename);
    if(savePath.isEmpty())
        return;
    UpdateServerInfo();
    XDiskClient::Get()->Download(filename.toUtf8().toStdString(), savePath.toUtf8().toStdString());
}
