#ifndef FTPSOCKET_H
#define FTPSOCKET_H

/**
* @brief: socket FTP请求
* @author: Fu_Lin
* @date:  2019-05-28
* @description: 支持断点续传下载，支持上传命令操作，
*                       更改目录新建目录，
*                       获取文件大小，列表
*/

#include <QObject>
#include <QSslSocket>
#include <QTcpSocket>
#include <QFile>
#include <QPair>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker >
#include <QPointer>
#define DOWNLOAD_FILE_SUFFIX    "_tmp"
#define DOWNLOADFILENAME "TempFile"
#define PAR_FILE_SUFFIX    "par"
//定时器触发时间
#define DELAY_TIME 50

//定义每个分段都是一个单独的线程运行下载,如果注释掉则采用分段下载集中在一个多线程中，均在run中进行
#define MultiThreadRunDown


class SegmentedDownload;

class FTPSocket : public QObject
{
    Q_OBJECT
public:
    explicit FTPSocket(QObject *parent = nullptr);
    ~FTPSocket();

    //ftp操作模式
    enum FTPMODE{
        MODE_Stream = 0,  //流模式，不写入文件，直接向外抛出，由外面做处理 (如果使用分段线程下载，此模式无效，请设置为 MODE_ThreadStream)
        MODE_FILE,  //文件模式，将下载的流写入本地文件， 默认文件模式
        MODE_ThreadStream  //线程流模式，不写入文件，直接向外抛出，由外面做处理,使用分段线程下载时设置此模式，详见 ftpStartSegmentedDownload 函数
    };

    /**
     * @brief setFtpUserNamePasswd  设置ftp的用户名和密码
     * @param userName  用户名
     * @param passwd 密码
     */
    void setFtpUserNamePasswd(QByteArray userName, QByteArray passwd);

    /**
     * @brief connectFtpServer 连接ftp的服务
     * @param hostName 主机名称
     * @param port 主机端口 (默认21)
     */
    void connectFtpServer(QString hostName, quint16 port = 21);

    /**
     * @brief connectFtpServer 重载函数 连接ftp服务
     * @param hostName 主机名称
     * @param port 主机端口
     * @param userName 用户名称
     * @param passwd 用户密码
     */
    void connectFtpServer(QString hostName, quint16 port ,QByteArray userName, QByteArray passwd);

    /**
     * @brief downloadFtpFile 下载FTP文件
     * @param fileName 文件名称，如果不知道可以通过listFtpFile，拿到对应的文件名
     */
    void downloadFtpFile(const QString &fileName);

    //终端数据连接程序
    void aborFtpDataConnect();

    //该方法会关闭数据连接通道并关闭客户端和服务端的连接，向服务端发送关闭信号
    void aborFtpAllConnect();
    //保存文件的输入流并关闭ftp连接(强烈建议使用此函数)
    void saveDownloadFile();

    //列出并返回所有的ftp文件夹中的文件名称,带后缀
    QStringList listFtpFile();

    //获取ftp文件的大小
    qint64 FtpFileSize(const QString & fileName);

    //切换ftp目录, dirName--目录的名称
    void FtpCWD(QString dirName);

    //打印当前的ftp目录
    void FtpPWD();

    //关闭ftp连接(强烈建议使用saveDownloadFile函数来关闭ftp的连接)
    void closeFTPConnect(bool synchronization=true);
    
    //设置ftp的模式（流模式还是文件模式）
    void setFtpMode(FTPSocket::FTPMODE ftpmode);

    void setFtpHostName(QString hostName);
    //设置ftp下载的文件名字
    void setFtpDownFileName(const QString & downFileName);

    void setFtpsegmentedDownloadPoint(QPair<qint64, qint64> segmentedDownloadPoin);

    //下载的文件大小，可以通过FtpFileSize获取，也可以外部设置
    void setFtpDwonFileSize(qint64 filesize);


public slots:
    /**
     * @brief ftpStartSegmentedDownload 开始分段下载,外面调用，单个文件下载时调用此方法
     * @param ftpHost  ftp主机
     * @param userName ftp 用户名
     * @param passwd ftp 密码
     * @param cdDir 需要跳转的目录,如果不需要跳转，请设置为空字符串
     * @param downfileName 下载的ftp文件名
     * @note 调用该方法，不用手动显示连接ftp，内部会重新连接，
     */
    void ftpStartSegmentedDownload(const QString &ftpHost, const QByteArray &userName, const QByteArray &passwd, const QString & cdDir, const QString &downfileName);

    /**
     * @brief ftpStartSegmentedDownload 重载方法， 一个文件被外面分割成多个小文件下载时，调用此方法
     * @param ftpHost ftp主机
     * @param userName ftp 用户名
     * @param passwd ftp 密码
     * @param cdDir  需要跳转的目录，如果不需要跳转，请设置为空字符串
     * @param downfileName 下载的ftp文件名
     * @note 调用该方法，不用手动显示连接ftp，内部会重新连接，
     */
    void ftpStartSegmentedDownload(const QString &ftpHost, const QByteArray &userName, const QByteArray &passwd, const QString & cdDir,const QStringList &downfileName);
    //中途取消,保存断点续传文件记录
    void ftpCloseSegmentedDownload();

    //!!!!!!!下面两个方法外面不要调用!!!!!!!
    /**
     * @brief startSegmentedDownload 设置分段下载的下载点，用于多线程分段下载
     * @param fileOffset 需要下载的文件偏移量
     * @param downfileName 需要下载的文件名字
     */
    void startSegmentedDownload(QString downfileName, qint64 downFileSize, QByteArray downData , QString cdDir, qint64 SegmenteddownPoint, QPair<qint64, qint64> segmentedDownloadPoint, QThread * thread = nullptr);


   void  startSegmentedDownload();

   /**
    * @brief writeCommand 写入需要的ftp命令
    * @param comm  ftp命令
    * @param synchronization 是否同步等待命令完成
    */
   void writeCommand(const QByteArray & comm, bool synchronization = true);
   void writeCommand(const QString & comm, bool synchronization = true);

   //设置FTP为被动模式
   void pasvMode();

private:
    //ftp操作的流程状态码
    enum FTPOperation{
        FTP_LISTFile = 0,
        FTP_DOWNFile,
        FTP_SegmentedDownload,
        FTP_UPLOADFile,
        FTP_GETFILESize
    };
    
  
    //切割计算被动模式的ip和端口，返回端口号
    int splitPasvIpPort(const QString &PASVPassiveStr);

    //监听ftp数据端口
    void listenFtpDataPort(quint16 port);
    

    //格式化获取文件的大小
    QPair<qint64, QString> formatFileSize(const qint64 &filesizebyte, int precision = 2);


signals:
    void sigFTPConnectFinish();  //ftp连接完成触发
    void sigFTPListFileFinish(); //ftp列表文件获取完成触发
    void sigFTPReconnectFaile(); //ftp重连失败触发
    void sigFTRConnectFaile(); //ftp连接失败触发
    void sigFTPPasvFinish(); //ftp 进入被动模式切换成功触发
    void sigFTPCommandFinish(); //ftp命令完成触发
    void sigDownloadProgress(qint64 bytesReceived, qint64 bytesTotal); //ftp 文件下载进度
    void sigDownloadSpeedUpdate(QString speed); //下载网速的更新
    void sigDownloadTimeUpdate(QString time); //下载剩余时间的更新
    void sigDownloadTimeout();  //下载超时

    void sigFTPDownloadFinish(QString filePath); //ftp下载完成并保存成功触发
    void sigFTPCloseConnect();
    void sigFTPStreamFileFinish(QByteArray fileStream); //ftp下载完成。发送内存流信号
    void sigFTPCWDSuccess();   //目录切换成功后触发
    void sigFTPPWDFinish(QString dirName); //查看当前目录成功后触发
    void sigDownFileFaile(QString faileMessage); //下载文件失败后触发

    void sigFtpConnectInterruption(); //一般是链接中断才触发此信号

    //分段下载完成后触发
    void sigFTPSegmentedDownloadData(QByteArray downData, qint64 bytesReceived);
    void sigFTPSegmentedDownloadEnd(); //分段下载结束

    //socket写信号
    void sigFtpSocketWrite(const QByteArray & socketData);
public slots:
    void onFtpConnectFinish();  //连接上ftp服务器后调用

    virtual void onReadData();  // 服务器通道连接读取数据
    virtual void onFtpDataRead();  //数据通道连接，读取数据，下载等用到

    void displayError(QAbstractSocket::SocketError socketError);


    //用于继承者单独访问
public:
    QTcpSocket * m_FtpServerSocket = nullptr; //服务通道
    QTcpSocket * m_FtpDataSocket =  nullptr; //数据通道
private:


    QByteArray m_ftpUserName; //记录当前的ftp用户名
    QByteArray m_ftpPasswd; //记录当前的ftp密码
    bool m_loginSuccess = false; //是否登录成功的标记
    QFile * m_pFile = nullptr; //操作下载文件保存到本地的文件指针
    QString m_dwfileName;  //当前下载文件的名称
    QString m_dwfilesuffix; //下载文件的后缀
    QPair<qint64, QString>  m_dwFileSizePair; //下载文件的大小, 第一个是纯大小，第二个是转换后的大小加单位

    bool m_isFileDownFinish = false;  //判断文件是否已经下载完成

    QString m_hostName;  //记录登录的主机名称
    quint16 m_hostPort; //记录主机的端口号

    QStringList m_ftpFileList;  //获取ftp的文件列表


    int m_reconnections = 0;  //重连接的次数
    int m_downFileReceived;  //当前文件收到的文件大小
    FTPOperation m_ftpEnumStatus;   //ftp操作的枚举状态码
    FTPMODE m_ftpEnumMode = FTPSocket::MODE_FILE ; //ftp模式 
    
   QByteArray m_fileCacheData = ""; //文件缓存的数据存储，需要最后关闭文件的时候写入文件


   qint64 m_fileCacheOffset = 0;  //文件的偏移量，断点续传会用到
   QPair<qint64, qint64> m_segmentedDownloadPoint;  //记录的分段下载点，开始下载和结束下载的分段点

   QPointer<SegmentedDownload> m_SegmentedDownload = nullptr;


   QThread *m_currentThread = nullptr;
   QMutex  m_mutex;
   QString m_cdDir;  //记录需要切换的目录

};

class SegmentedDownload: public QObject{

    Q_OBJECT

public:
    explicit SegmentedDownload(QObject *parent = nullptr);
    ~SegmentedDownload();

    //ftp操作模式, 有外面ftp传入
    enum FTPMODE{
        MODE_Stream = 0,  //流模式，不写入文件，直接向外抛出，由外面做处理
        MODE_FILE  //文件模式，将下载的流写入本地文件， 默认文件模式
    };
    //设置ftp的model，文件还是流下载，流下载不能断点续传
    void setFtpMode(FTPMODE ftpModel);
    /**
     * @brief setMaxThreadNum 设置最大的线程数
     * @param threadNum 线程数的个数，不设置，默认是5个(最佳)
     * @note 并不是越大越好，通过使用默认提供线程是最佳方案，
     *           当然可以采用QThread::idealThreadCount()来动态获取最佳线程数
     */
    void setMaxThreadNum(int threadNum);
    /**
     * @brief setDownFileSize 设置下载文件的大小
     * @param fileSize 文件大小
     */
    void setDownFileSize(const qint64 &fileSize);

    /**
     * @brief setDownFileName 设置下载文件的名字
     * @param fileName 设置文件的名字
     */
    void setDownFileName(const QString &fileName);

    /**
     * @brief setCDFileDirName 设置ftp需要切换的目录名称
     * @param dirName
     */
    void setCDFileDirName(const QString & dirName);
    /**
     * @brief setDownFileName 设置下载的文件名字
     * @param fileNameList 设置被外界分割好的文件list名称
     */
    void setDownFileName(const QStringList & fileNameList);
    /**
     * @brief startThreadDownload  启动线程下载，不管外面是直接调用还是start都是线程启动，
     * note: 强烈建议此类最好不要在外面类单独调用，而是通过FTPSocket ---ftpStartSegmentedDownload方法来调用
     */
    void startThreadDownload();

    /**
     * @brief setFtpHostName  设置ftp主机的名称， 用于多线程下载备用
     * @param hostName //主机的名称
     */
    void setFtpHostName(const QString & hostName);

    /**
     * @brief setFtpUserNamePass 设置ftp的用户名和密码
     * @param userName 登录用户名
     * @param passwd 登录密码
     */
    void setFtpUserNamePass(const QByteArray &userName, const QByteArray &passwd);


    void closeResumeFile();

private:
    //单独连接一下ftp来获取下载文件的大小
    void getDownFileSize();

    //返回保存文件的位置
    QDir saveDownFileDir();

    //读取断点续传，判断是否有缓存文件，有，说明要启动断点续传，没有直接下载
    bool readResumeFile();

protected:
    //采用单线程运行多段下载，注：必须注释掉宏MultiThreadRunDown，才会被自动调用
    //virtual void run();
    //初始化所有的变量，清空还原所有的数据默认值
     void initAllListData();
private slots:

     void onReadyDownFile();
     void onStartFtpConnect();
     void onUpdateDownloadSpeed();
     void onDownloadDataFinish();  //下载数据完成后调用，统一写入文件
     void onDownloadDataUpdate(QByteArray downData,  qint64 bytesReceived);

     //下载超时时触发
     void onDownloadTimeout();
     //! ftp关闭连接后触发
     void onFtpClostConnect();
 signals:
    void sigStartFtpConnect(QString ftpHost, quint16 port, QByteArray ftpUserName, QByteArray ftpUserPasswd);
    void sigDownFile(QString downfileName,  qint64 downFileSize, QByteArray downData,  QString cdDir, qint64 downPoint,  QPair<qint64, qint64> dwFileSection,QThread * thread);
    void sigDownFileFaile(QString message);
    void sigDownloadProgress(qint64 bytesReceived,qint64 bytesTotal);
    void sigDownloadSpeedUpdate(QString speed); //下载网速的更新
    void sigDownloadTimeUpdate(QString time); //下载剩余时间的更新
    void sigCloseFtpConnect(); //关闭ftp连接
    void sigDownStreamFileFinish(QByteArray fileStream); //流模式下载文件完成
    void sigDownFinish(QString filePath); //分段下载全部完成
    void sigDownLoadTimeout();  //下载超时触发。服务器那边没回响超过设定时间触发
    void sigaborFtpConnect();
private:
    QList<QThread*> m_ListThread;

    QList<FTPSocket *> m_FTPSocketList;  //记录所有的ftpsocker后面结束要手动析构
    int m_maxThreadNum =5;//QThread::idealThreadCount();   //默认开5个线程，稳定分段下载，大量测试可以快1/3的速度

    QString m_hostName; //记录FTP主机的名字
    QByteArray m_userPass; //记录用户的密码
    QByteArray m_userName; //记录用户的姓名
    qint64 m_downReceivedSize = 0; //记录接收到的下载的所有总长度，提供给进度条使用，没有用到

    QList<QByteArray> m_listFtpData;
    QList<qint64> m_ProgresslistByte;
    qint64 m_downFileSize = 0;  //下载文件的种大小
    QList<qint64> m_downFileSizeList; //记录外界传入的各个分区文件大小
    QString m_downFileName;   //记录下载的名字，自己分区par文件下载
    QStringList m_downFileNameList; //有外界传入的list文件名称，由外界分割par文件，这边下载整理即可，不需要分割

    QList<QPair<qint64, qint64>> m_listDownSegmentedInterval; //下载的分段区间,起到区间判别的作用

    QList<qint64> m_SegmentedPoint;   //分段的下载点，起到断点续传的作用

    QList<bool> m_downFinish;

    QMutex m_mutex;

    qint64 m_progressBytes = 0; //下载接收的总数据大小
     int m_isAllFinish = 1; //所有文件完成的标记，只有标记和写入的一致时才将数据写入

     int m_TotalTime = 0;  //记录下载时花费的上一个时间
     int m_nTime = 0; //记录当前的结束时间
     QTimer m_downUpdateTimer;  //下载更新定时器
     QTime m_downElapsedTime; //下载更新计时器
     int m_downElapsedTimeCacle = 0; //下载更新的时间缓存
     QList<QFile *> m_fileResumeList; //断点续传文件的使用 , 只有 FTPMODE::MODE_FILE 才使用

     qint64 m_DownBlock = 0;  //下载的文件块

     QString m_cdDirName; //需要切换的目录名称


     FTPMODE m_ftpModel = FTPMODE::MODE_FILE;
     QTime m_downTime; //下载计时，总共耗时多久(测试使用，没有实际用途)


     int m_ftpCloseNum = 0; //ftp关闭的数量，只有关闭和打开的数量一致时，才将数据写入文件或者向外抛出，
                                         //否则在没有关闭之前抛出数据，因为流程的走向可能会出现不可预估的错误


     bool m_isDestory = false; //是否类被析构，防止被析构后还在运行
     QTimer m_downTimeoutTimer; //下载超时时触发
     int m_downTimeout = 10000; //下载超时的时间，毫秒为单位，5000 = 5秒， 10秒没反应则触发超时信号

};


#endif // FTPSOCKET_H
