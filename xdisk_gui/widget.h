#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void UpdateServerInfo();

public slots:
    void Refresh();
    void UpdateDir(std::string dirs);
    // 用户选择一个文件，插入到列表
    void Upload();
    // 用户选择一个文件进行下载
    void Download();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
