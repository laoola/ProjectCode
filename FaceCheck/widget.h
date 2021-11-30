#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//opencv include
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include <QCamera>
#include <QVideoProbe>
#include <QAbstractVideoSurface>
#include <QVideoProbe>
#include <QThread>
#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QQueue>
#include <QCamera>
#include <QPen>
#include <QPainter>
#include <QRgb>
#include <QDateTime>
#include <QCameraInfo>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QComboBox>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void opencv_face(QImage qImage);
    ~Widget();
    void CreateSignal();
    QList<QCameraInfo> video_dev_list;
    void show_face(IplImage* img);
    class VideoReadThread_0 *videoRead_WorkClass_0;  //视频工作类
    QThread *videoRead_Workthread_0; //视频线程
    /*驾驶室摄像头*/
    void Stop_VideoAudioEncode_0(bool flag);//停止线程
    void Start_VideoAudioEncode_Thread_0(); //启动线程
    void Log_Text_Display(QPlainTextEdit *plainTextEdit_log,QString text);
    void UpdateVideoAudiodDevice(QComboBox *comboBox_video, QPlainTextEdit *plainTextEdit_log);
    IplImage *QImageToIplImage(const QImage * qImage);
    QImage *IplImageToQImage(IplImage *img);
    QTimer timer;
signals:
    void Init_VideoAudio_WorkClass_0();
    void Stop_AudioVideo0_work_0();
private slots:
    void update();
    void on_pushButton_Start_clicked();
    void Log_Display_0(QString text);
    void VideoDataDisplay_0(QImage );
    void on_pushButton_update_clicked();

    void on_pushButton_stop_clicked();

private:
    Ui::Widget *ui;
};

class VideoReadThread_0:public QObject
{
    Q_OBJECT
public:
    QCamera *camera;
    QVideoProbe *m_pProbe;
    VideoReadThread_0(QObject* parent=nullptr):QObject(parent){camera=nullptr;m_pProbe=nullptr;}
    ~VideoReadThread_0();
    void Camear_Init(void);
public slots:
    void stop();
    void run();
    void slotOnProbeFrame(const QVideoFrame &frame);
signals:
    void VideoDataOutput(QImage); //输出信号
};
//视频音频编码类
class VideoAudioEncode
{
public:
    QCameraInfo camera; //当前选择的摄像头
};
//视频输出尺寸
#define VIDEO_WIDTH  640
#define VIDEO_HEIGHT 480
extern class VideoAudioEncode videoaudioencode_0;
#endif // WIDGET_H
