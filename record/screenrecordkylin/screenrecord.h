#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <gif.h>
#include <QScopedPointer>
#include <QRect>
#include <QTimer>


class ScreenRecord :public QObject
{
    Q_OBJECT

public:
    explicit ScreenRecord();
    ~ScreenRecord();

    void SetConf(QString strTime, QString strFile);
    bool CheckTime();
    void record();

protected slots:
    void saveImage();

private:

    int fps;                    //帧数 100为1s
    int count;                  //帧数计数
    int m_nContineTime;           //录像持续时间
    long m_tStartTime;        //开始录像时间
    QString fileName;           //保存文件名称
    QRect rectGif;              //截屏区域
    QTimer *m_pTimer;              //截屏定时器

    Gif gif;                    //gif类对象
    Gif::GifWriter *gifWriter;  //gif写入对象
};

#endif // SCREENWIDGET_H
