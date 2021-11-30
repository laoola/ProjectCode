#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//opencv include
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>


#include <QMainWindow>
#include <QWidget>
#include <QCameraInfo>
#include <QCamera>
#include <QVideoWidget>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void CreateSignal();

    void opencv_face(QImage qImage);
    void show_face(IplImage* img);
    IplImage *QImageToIplImage(const QImage * qImage);
    QImage *IplImageToQImage(IplImage *img);

private slots:
    void slot_start_clicked();

    void slot_stop_clicked();

    void slot_capture_clicked();
    //显示图片
    void show_pic(int id, const QImage &preview);
    void slot_TimeOut();

private:
    Ui::MainWindow *ui;
    QCamera *ca;//摄像头对象
    //QVideoWidget *w;
    QCameraViewfinder *w;
    QCameraImageCapture *cap;//截屏对象
    QTimer timer;
};
#endif // MAINWINDOW_H
