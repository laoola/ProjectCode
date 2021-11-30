#ifndef FTPMULTITHREADDOWNLOAD_H
#define FTPMULTITHREADDOWNLOAD_H

/**
* @brief: Ftp多线程下载专用 (支持分段下载)
* @author: Fu_Lin
* @date:  2019-10-12
* @details: 继承ftpsocket，一旦继承就是多线程分段下载
* @note: 注意
*/

#include <FTPSocket.h>
#include <QFile>
#include <QDir>
#include <QTime>

class FTPClient;
class QThread;

class FtpMultithreadDownLoad : public QObject
{
    Q_OBJECT
public:

    enum class FTPMODE{
        MODE_FILE,  //文件模式，将下载的流写入本地文件， 默认文件模式
        MODE_ThreadStream  //线程流模式，不写入文件，直接向外抛出，由外面做处理,使用分段线程下载时设置此模式，详见 ftpStartSegmentedDownload 函数
    };

    explicit FtpMultithreadDownLoad(QObject *parent = nullptr);

    ~FtpMultithreadDownLoad();
    //设置请求模式和线程数，默认是5
    explicit FtpMultithreadDownLoad(FtpMultithreadDownLoad::FTPMODE ftpMode, int maxThreadNum = 5);

//    enum class FTPMODE{
//        MODE_FILE,  //文件模式，将下载的流写入本地文件， 默认文件模式
//        MODE_ThreadStream  //线程流模式，不写入文件，直接向外抛出，由外面做处理,使用分段线程下载时设置此模式，详见 ftpStartSegmentedDownload 函数
//    };

    void setFtpRequestMode(FtpMultithreadDownLoad::FTPMODE ftpMode);

    void setDownMaxThreadNum(int threadNum);

    //文件分段准备
    void fileSegmentation();

    int getDownFileSize(const QString &ftpHost, const QString &ftpUserName, const QString &ftpPasswd , const QString &cddirName, const QStringList &downFileName);

    QDir saveDownFileDir();

    //合并所有下载好的小文件
    void mergeAllParFile();

    //初始化下载时间和速度的计算
    void initdownTimerSpeedTimer();

    /**
     * @brief ftpStartDown ftp开始多线程下载
     * @param ftpHost ftp的主机名
     * @param ftpUserName ftp的用户名
     * @param ftpPasswd ftp的用户密码
     * @param chdirName 需要切换的ftp目录名称 (为空就是不切换)
     * @param downFileName ftp需要下载的名称，单独一个文件
     */
    void ftpStartDown(const QString &ftpHost, const QString &ftpUserName, const QString & ftpPasswd, const QString & chdirName, const QString & downFileName);

    /**
     * @brief ftpStartDown 重载上面的方法
     * @param ftpHost
     * @param ftpUserName
     * @param ftpPasswd
     * @param chdirName
     * @param downFileNameList  ftp需要下载的名称，由外界已经分隔好的小文件
     */
    void ftpStartDown(const QString &ftpHost, const QString &ftpUserName, const QString & ftpPasswd, const QString & chdirName, const QStringList & downFileNameList);

signals:
    /**
     * @brief sigConnectFtpClient  连接ftp下载客户端，并自动进行断点续传下载触发信号
     * @param ftpHost ftp主机名称
     * @param ftpUserName ftp用户名称
     * @param ftpPasswd ftp用户密码
     * @param chdirName ftp需要切换的目录 (为空就是不切换)
     * @param breakpointdown 断点续传记录的下载点和数据
     *               QPair<QByteArray, int> ，第一个参数是已经下载的数据， 第二个是需要从哪里开始继续下载的地址
     *               如果是首次下载，一个为空，一个为0
     * @param segmentedDownloadPoint 分段下载的开始大小和结束大小  QPair<int, int>
     * @param threadID 多线程需要用到的线程id，起到防伪多次进来的作用
     */

    // const QPair<QByteArray, int> & breakpointdown, const QPair<int, int> & segmentedDownloadPoint,
    void sigConnectFtpClient(const QString &ftpHost,  const QString  &ftpUserName,  const QString & ftpPasswd,
                              const QString & chdirName, const  QString  &downFileName,  QThread * threadID);

    //更新当前的下载的进度，公式可以是bytesReceived/bytesTotal * 100
    void sigDownProgressUpdate(qint64 bytesReceived, qint64 bytesTotal);

    //更新当前的下载速度和剩余的时间
    void sigDownSpeedTimeUpdate(QString curDownSpeed, QString remainingTime);

    void sigDownFinish(QString fileSavePath);

    void sigDownStreamFinish(const QByteArray  &fileStreamData);

    //请求连接错误和给定的文件大小为0时触发
    void sigFtpConnectFaile();

    //ftp请求文件大小为0时触发，一般于文件不存在才会这样
    void sigFtpFileError(QString msg);
    //下载中断无响应触发
    void sigFtpDownNoResponse();

public slots:

     void onFtpDownFileUpdateState(int downSize, bool isFinish);

     //下载速度和时间的计算
     void onDownSpeedTime();
private:

    QString m_downFileName;
    QStringList m_downFileNameList;  //有外界分割后的小文件的下载名称

    int m_maxThreadNum = 5; //默认最多5个线程同时下载
    int m_oldMaxThreadNum = 5; //记录外面被手动设置后的记录，用于复位设置，默认外面不手动设置就不用管
    int m_downFinishNum = 0;//记录下载完成的数量，需要和m_maxThreadNum匹对

    QList <FTPClient*> m_ftpClient;
    QList <QThread*> m_thread;

    int m_downFileSize = 0;  //保存下载文件的总大小大小
    QList<int> m_downFileSizeList; //下载文件的大小，外面已经分隔好的文件的大小
    bool m_isFileSegmented = true; //是否启动文件分段工作， 如果有外界传入的文件已经分段，则不需要
    QList<QPair<int, int> > m_segmentedDownloadPointList; //下载的分段点，第一个是开始，第二个是结束

    QList<QPair<QByteArray, int>> m_breakpointdownList;

    QStringList m_downTempFileNameList;  //所有保存的临时文件名，合并时会用到

    QList<int> m_progressSizeList; //保存每一个线程的进度大小

    bool m_ftpConnectFaile = false;  //判断连接是否失败

    // 总时间
    int m_nTime  = 0; //文件下载花废的总时间
    QTime m_downloadTime; //下载计时，计时器，不是定时器
    QTimer m_downTimer; //下载更新定时器，更新下载速度，剩余时间，百分比等

    QTime m_downTestTotalTime; //下载总共话费的时间，测试用
    int m_bytesReceivedSize = 0; //保存接收数据的大小，只是大小，不是流数据

    FTPMODE m_ftpRequestMode = FTPMODE::MODE_FILE; //请求模式

};

//重新实现ftp的下载读写数据的操作
class  FTPClient: public FTPSocket
{
    Q_OBJECT
public:

    explicit FTPClient(QObject *parent = nullptr);
    explicit FTPClient(QPair<QByteArray, int> breakpointdown,  QPair<int, int> segmentedDownloadPoint, QString parfileName);
    ~FTPClient();
    //因为是多线程调用下载，所以重新实现读写数据的操作
   virtual void onFtpDataRead();


    //需要保存的下载文件的目录
    QDir saveDownFileDir();

public slots:
    /**
     * @brief onConnectFtpClient  连接ftp下载客户端，并自动进行断点续传下载
     * @param ftpHost ftp主机名称
     * @param ftpUserName ftp用户名称
     * @param ftpPasswd ftp用户密码
     * @param chdirName ftp需要切换的目录 (为空就是不切换)
     * @param downFileName 需要下载的文件的名称
     * @param breakpointdown 断点续传记录的下载点和数据
     *               QPair<QByteArray, int> ，第一个参数是已经下载的数据， 第二个是需要从哪里开始继续下载的地址
     *               如果是首次下载，一个为空，一个为0
     * @param segmentedDownloadPoint 分段下载的开始大小和结束大小  QPair<int, int>
     * @param threadID 多线程需要用到的线程id，起到防伪多次进来的作用
     */
    void onConnectFtpClient(const  QString &ftpHost,  const QString &ftpUserName,  const QString & ftpPasswd,
                             const QString &  chdirName, const QString & downFileName,  QThread * threadID); // const QPair<QByteArray, int> & breakpointdown, const QPair<int, int> & segmentedDownloadPoint,


    void onFtpDownFile();

    //void onFtpReconnect(); //FTP断开，启动重连操作

signals:
    void sigDownProgressBytes(int downSize, bool isFinish);
private:

    QFile m_fileHand;  //文件句柄，读写操作
    QByteArray m_ftpCacheData = "";
    QDir m_downFileDir;
    QPair<int, int> m_segmentedDownloadPoint; //下载的分段点，第一个是开始，第二个是结束

    QPair<QByteArray, int> m_breakpointdown;  //断点续传需要用到的数据
    QString m_cdDirName = ""; //记录需要切换的Ftp目录名称，为空不切换

    QString m_downFtpFileName;  //需要下载的ftp文件名称

     bool m_deleteSelf= false;  //是否开始进行析构

};

#endif // FTPMULTITHREADDOWNLOAD_H
