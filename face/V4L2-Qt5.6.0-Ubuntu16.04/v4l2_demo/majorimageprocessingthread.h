#ifndef MajorImageProcessingThread_H
#define MajorImageProcessingThread_H

#include <QThread>
#include <QImage>
#include <QDebug>

#include "LPF_V4L2.h"

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
#include <QDebug>

class MajorImageProcessingThread : public QThread
{
    Q_OBJECT
public:
    MajorImageProcessingThread();

    QImage majorImage;
    void stop();
    void init(int index);

protected:
    void run();

private:
    volatile int majorindex;
    volatile bool stopped;

signals:
    void SendMajorImageProcessing(QImage image, int result);
    void SendImgaeFaceCheck(QImage image);
};

class FaceCheckThread :public QThread
{
    Q_OBJECT
public:
    FaceCheckThread();

    void stop();
    void opencvFace(QImage qImage);
    void showFace(IplImage* img);
    IplImage *QImageToIplImage(const QImage * qImage);
    QImage *IplImageToQImage(IplImage *img);


    QTimer timer;
    bool bStop;
protected:
    void run();

public slots:
    void slot_setCheckImage(QImage image);

signals:
    void SendFaceCheckRlt(QImage image);
private:
    QImage m_qImage;
};

#endif // MajorImageProcessingThread_H
