#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <opencv2/opencv.hpp>
#include "blockingconcurrentqueue.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace moodycamel;
QT_END_NAMESPACE
void image_filter_thread();
class WorkThread : public QThread
{

private:
    void run()
    {
        image_filter_thread();
    }
public:

signals:

public slots:

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
   void onCapTimerUpdate( void );
   void ondisTimerUpdate(void);
private slots:
   void on_th1_valueChanged(int value);

   void on_th2_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QTimer              *m_cap_update_timer;
    QTimer              *m_cap_update_timer2;
    cv::VideoCapture     m_capture;
    QGraphicsScene      *m_graphics_scene1,*m_graphics_scene2,*m_graphics_scene3;



};



#endif // MAINWINDOW_H
