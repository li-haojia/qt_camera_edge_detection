/**
  * author:Li Haojia
  * data:2020-1-13
  * brief: edge decetiction
  *
  *
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <QThread>
#define RGB_GRAY 1
BlockingConcurrentQueue<cv::Mat> qq1;
BlockingConcurrentQueue<cv::Mat> qq2;
int a1,a2;


void image_filter_thread()
{
    cv::Mat raw,raw1,filtered;
    while(1)
    {
    if(qq1.try_dequeue(raw))
    {
        cv::GaussianBlur(raw, raw1, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
        //先使用3*3内核来降噪
        //cv::blur(raw, filtered, cv::Size(3, 3));
        Canny(raw1, filtered, a1, a2, 3);
        qq2.enqueue(filtered);

    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    m_cap_update_timer = new QTimer(this);
    connect(m_cap_update_timer, SIGNAL(timeout()), this, SLOT(onCapTimerUpdate()));
    m_cap_update_timer->start(30);
    m_cap_update_timer2 = new QTimer(this);
    connect(m_cap_update_timer2, SIGNAL(timeout()), this, SLOT(ondisTimerUpdate()));
    m_cap_update_timer2->start(30);
    m_graphics_scene1 = new QGraphicsScene(this);
    m_graphics_scene2 = new QGraphicsScene(this);
    m_graphics_scene3 = new QGraphicsScene(this);
    m_capture.open(0);
    a1=ui->th1->value();
    a2=ui->th2->value();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ondisTimerUpdate()
{
    cv::Mat filtered;

    if(qq2.try_dequeue(filtered))
    {
        QImage image3(filtered.data, filtered.cols, filtered.rows, QImage::Format_Grayscale8);
        m_graphics_scene3->clear();
        m_graphics_scene3->addPixmap(QPixmap::fromImage(image3));
        ui->graphicsView3->setScene(m_graphics_scene3);
    }
}

void MainWindow::onCapTimerUpdate( void )
{
    cv::Mat camera_img;

    m_capture.read(camera_img);
    if(camera_img.empty())
    {
        return;
    }

#if RGB_GRAY
    // displayt rgb color image.
    cv::Mat rgb_img;
    cv::cvtColor(camera_img, rgb_img, cv::COLOR_BGR2RGB);

    QImage image1(rgb_img.data, rgb_img.cols, rgb_img.rows, QImage::Format_RGB888);

    cv::Mat gray;
    cv::cvtColor(camera_img, gray, cv::COLOR_RGB2GRAY);
#else
    // display grayscale image.
    cv::Mat gray;
    cv::cvtColor(camera_img, gray, cv::COLOR_RGB2GRAY);
    QImage image1(gray.data, gray.cols, gray.rows, QImage::Format_Grayscale8);
#endif

        qq1.enqueue(rgb_img);

        m_graphics_scene1->clear();
        m_graphics_scene1->addPixmap(QPixmap::fromImage(image1));
        ui->graphicsView1->setScene(m_graphics_scene1);


}

void MainWindow::on_th1_valueChanged(int value)
{
    a1=value;
}

void MainWindow::on_th2_valueChanged(int value)
{
    a2=value;
}
