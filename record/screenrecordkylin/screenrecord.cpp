#include "screenrecord.h"

#include <QMutex>
#include <QScreen>
#include <QDateTime>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>

long GetSysRunTime(bool bType)
{
    struct timespec times = {0, 0};
    long time;

    clock_gettime(CLOCK_MONOTONIC, &times);

    if (bType)
    {
        time = times.tv_sec;
    }
    else
    {
        time = times.tv_sec * 1000 + times.tv_nsec / 1000000;
    }

    return time;
}

ScreenRecord::ScreenRecord():m_pTimer(NULL), gifWriter(NULL)
{
    if(NULL == m_pTimer)
    {
        m_pTimer = new QTimer();

        connect(m_pTimer, SIGNAL(timeout()), this, SLOT(saveImage()));
    }

    rectGif = QRect(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
    m_tStartTime = GetSysRunTime(false);
}

ScreenRecord::~ScreenRecord()
{
    if(NULL != m_pTimer)
    {
        m_pTimer->stop();
        delete m_pTimer;
        m_pTimer = NULL;
    }

    if (0 != gifWriter)
    {
        delete gifWriter;
        gifWriter = 0;
    }
}


void ScreenRecord::SetConf(QString strTime, QString strFile)
{
    fps = 10;
    m_nContineTime = strTime.toUInt();
    fileName = strFile;
}

QString GetStringByTime(time_t time)
{
    struct tm *p;

    p = localtime(&time);
    char strtime[64] = {0};
    sprintf(strtime, "%d-%02d-%02d %02d:%02d:%02d", (1900 + p->tm_year),
            (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min,p->tm_sec);

    return strtime;
}



bool ScreenRecord::CheckTime()
{
    long tCurTime = GetSysRunTime(false);

    printf("%ld %ld\n", tCurTime, m_tStartTime);
    if(tCurTime - m_tStartTime > m_nContineTime*1000)
    {
        return false;
    }

    return true;
}


void ScreenRecord::saveImage()
{
    if (!gifWriter || !CheckTime())
    {
        gif.GifEnd(gifWriter);

        delete gifWriter;
        gifWriter = 0;
        printf("exit\n");

        exit(0);
    }

    QScreen *screen = QApplication::primaryScreen();
    QPixmap pix = screen->grabWindow(0, rectGif.x(), rectGif.y(), rectGif.width(), rectGif.height());
    QImage image = pix.toImage().convertToFormat(QImage::Format_RGBA8888);

    gif.GifWriteFrame(gifWriter, image.bits(), rectGif.width(), rectGif.height(), fps);
    count++;
    printf("%d\n", count);
}

void ScreenRecord::record()
{
    if (NULL != gifWriter)
    {
        delete gifWriter;
        gifWriter = NULL;
    }

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        fileName += ".gif";
    }

    gifWriter = new Gif::GifWriter;
    bool bOk = gif.GifBegin(gifWriter, fileName.toLocal8Bit().data(), rectGif.width(), rectGif.height(), fps);
    if (!bOk)
    {
        delete gifWriter;
        gifWriter = 0;
        return;
    }

    count = 0;
    m_pTimer->start(10);
}


