#include "mainwindow.h"

#include <QApplication>
#include <qdebug.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    WorkThread *task = new WorkThread; // 创建一个 WorkThread 对象
    task->start();
    qDebug()<<"Process thread Start";

    w.show();

    return a.exec();
}
