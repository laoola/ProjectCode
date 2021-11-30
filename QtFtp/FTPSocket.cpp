#include "FTPSocket.h"
#include <qdebug.h>
#include <QFileInfo>
#include <QDir>
#include <QEventLoop>
#include <QtMath>
#include <QCoreApplication>
#include "HelpClass.h"
#include <QMetaType>
#include <QTime>
#include "HelpClass.h"

FTPSocket::FTPSocket(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QPair<qint64, qint64>>("QPair<qint64, qint64>");
}

FTPSocket::~FTPSocket()
{
    qInfo()<<"进来析构FTP";
    if(m_SegmentedDownload)
    {
#ifndef MultiThreadRunDown
        //        m_SegmentedDownload->quit();
        //        m_SegmentedDownload->wait();
#endif
        delete m_SegmentedDownload;
        m_SegmentedDownload = nullptr;
    }
    if (m_FtpServerSocket)
    {
        m_FtpServerSocket->close();
        m_FtpServerSocket->deleteLater();
        m_FtpServerSocket = nullptr;
    }

    if (m_FtpDataSocket)
    {
        m_FtpDataSocket->close();
        m_FtpDataSocket->deleteLater();
        m_FtpDataSocket = nullptr;
    }
}

void FTPSocket::setFtpUserNamePasswd(QByteArray userName, QByteArray passwd)
{
    m_ftpUserName  ="USER "+userName+"\r\n";
    m_ftpPasswd ="PASS " + passwd+"\r\n";
}

void FTPSocket::connectFtpServer(QString hostName, quint16 port)
{

    if(m_FtpServerSocket == nullptr)
    {
        m_FtpServerSocket = new QTcpSocket;
    }
    else
    {
        return;
    }
    qInfo()<<"连接的主机是"<<hostName;
    m_hostName = hostName;
    m_hostPort = port;

    m_FtpServerSocket->abort();

    //    QSslConfiguration m_sslConfig = QSslConfiguration::defaultConfiguration();
    //    m_sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    //    m_sslConfig.setProtocol(QSsl::TlsV1SslV3);
    //    m_FtpServerSocket->setSslConfiguration(m_sslConfig);
    m_FtpServerSocket->connectToHost(m_hostName, port);


    connect(m_FtpServerSocket, &QTcpSocket::connected, this, &FTPSocket::onFtpConnectFinish);
    connect(m_FtpServerSocket, &QTcpSocket::readyRead, this, &FTPSocket::onReadData); //Qt::DirectConnection
    connect(this,QOverload<const QByteArray &>::of(&FTPSocket::sigFtpSocketWrite), m_FtpServerSocket, QOverload<const QByteArray &>::of(&QTcpSocket::write));

    connect(m_FtpServerSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this,
            [=](QAbstractSocket::SocketError socketError){ /* ... */

         qInfo()<<"socketError"<<socketError;
        if(socketError == QAbstractSocket::RemoteHostClosedError)
        {
            return;
        }
        emit sigFTRConnectFaile();

    });

}

void FTPSocket::connectFtpServer(QString hostName, quint16 port, QByteArray userName, QByteArray passwd)
{
    m_ftpUserName  ="USER "+userName+"\r\n";
    m_ftpPasswd ="PASS " + passwd+"\r\n";

    this->connectFtpServer(hostName, port);
}

void FTPSocket::downloadFtpFile(const QString & fileName)
{
    //获取文件大小
    FtpFileSize(fileName);

    //设置状态流程
    m_ftpEnumStatus = FTPSocket::FTP_DOWNFile;

    qInfo()<<"文件的后缀"<<m_dwfilesuffix;
    writeCommand(QByteArray("TYPE I\r\n"), false);

    //如果是文件模式则进行文件预处理
    if(m_ftpEnumMode == FTPSocket::MODE_FILE)
    {
        qInfo()<<"文件模式，打开文件";
        //设置下载的位置
        QString dirPath = HelpClass::getCurrentTempDataDir();
        dirPath = dirPath+"/downFile";
        qInfo()<<"dirPath"<<dirPath;
        QDir dir(dirPath);
        if(!dir.exists())
        {
            if(!dir.mkpath(dirPath))
            {
                qInfo()<<"下载路径创建失败"<<dirPath;
            }
        }

        QFileInfo fileInfo(fileName);

        m_dwfileName = fileInfo.fileName();
        m_dwfilesuffix = fileInfo.suffix();

        m_dwfileName.remove(m_dwfilesuffix);

        m_dwfileName = m_dwfileName + DOWNLOAD_FILE_SUFFIX;

        qInfo()<<"准备下载的文件的名字"<<m_dwfileName;

        QFileInfo info;
        info.setFile(dir, m_dwfileName);
        if(m_pFile == nullptr)
        {
            m_pFile = new QFile(info.filePath());
        }

        //qInfo()<<"file size"<<info.size();
        m_fileCacheOffset = info.size();
        qInfo()<<"下载文件的大小. = "<<m_dwFileSizePair.first;
        if(m_fileCacheOffset > 0 && m_fileCacheOffset != m_dwFileSizePair.first)
        {
            m_pFile->open(QIODevice::Append);
            qInfo()<<"启动断点续传下载.";
            writeCommand(QString("REST %1\r\n").arg(m_fileCacheOffset));
        }
        else {
            if(!m_pFile->open(QIODevice::WriteOnly))
            {
                qInfo()<<"文件打开失败.";
            }
        }
    }

    pasvMode();

    qInfo()<<"准备下载的文件."<<fileName;
    writeCommand(QString("RETR %1\r\n").arg(fileName));
}

void FTPSocket::aborFtpDataConnect()
{
    if(m_FtpDataSocket)
    {
        m_FtpDataSocket->close();
        m_FtpDataSocket->abort();
    }

}

void FTPSocket::aborFtpAllConnect()
{
    aborFtpDataConnect();
    closeFTPConnect();

}

void FTPSocket::saveDownloadFile()
{

    if(m_ftpEnumMode == FTPSocket::MODE_Stream)
    {
        emit sigFTPStreamFileFinish(m_fileCacheData);
    }
    else
    {
        if(m_pFile)
        {
            if(m_pFile->isOpen())
            {
                qInfo()<<"文件打开，写入文件.";
                m_pFile->write(m_fileCacheData);
                m_pFile->flush();

                //    m_pFile->rename(m_pFile->fileName(),m_dwfileName+m_dwfilesuffix);
                m_pFile->close();

                //            qInfo()<<"oldName= "<<m_pFile->fil();
                //             qInfo()<<"NewName= "<<m_dwfileName+m_dwfilesuffix;
                if(m_isFileDownFinish)
                {
                    m_isFileDownFinish = false;
                    QFileInfo fileInfo(*m_pFile);
                    QString filePath = fileInfo.absolutePath();
                    qInfo()<<"filePath" <<filePath;
                    QString fileOldSuffix = fileInfo.suffix();
                    qInfo()<<"fileOldSuffix" <<fileOldSuffix;
                    m_dwfileName =  m_dwfileName.remove(fileOldSuffix);
                    qInfo()<<"m_dwfileName" <<m_dwfileName;
                    QString fileNewName= filePath+ "/."+m_dwfileName+m_dwfilesuffix;
                    qInfo()<<"fileNewName" <<fileNewName;
                    qInfo()<<"oldName"<<m_pFile->fileName();

                    if(QFile::exists(fileNewName))
                    {
                        QFile::remove(fileNewName);
                    }
                    m_pFile->rename(m_pFile->fileName(), fileNewName);


                    emit sigFTPDownloadFinish(fileNewName);
                }
            }
            delete m_pFile;
            m_pFile =nullptr;
        }
    }

    //关闭FTP的连接，延时500毫秒
    QTimer::singleShot(DELAY_TIME, this, [=]{
        qInfo()<<"进来关闭Ftp.";
        closeFTPConnect();
    });
}

void FTPSocket::listenFtpDataPort(quint16 port)
{
    //m_loginSuccess = true;
    //开始监听数据端口
    if(m_FtpDataSocket == nullptr)
    {
        m_FtpDataSocket  = new QTcpSocket();
    }
    m_FtpDataSocket->close();
    m_FtpDataSocket->abort();
    m_FtpDataSocket->connectToHost(m_hostName ,port);

    connect(m_FtpDataSocket, &QTcpSocket::readyRead, this, &FTPSocket::onFtpDataRead,Qt::DirectConnection);

    //ftpPasv完成
    emit sigFTPPasvFinish();
}

void FTPSocket::pasvMode()
{
    QByteArray comm = "PASV\r\n";
    //m_FtpServerSocket->write(comm);
    emit sigFtpSocketWrite(comm);
    QEventLoop loop;
    connect(this, &FTPSocket::sigFTPPasvFinish, &loop, &QEventLoop::quit);

    loop.exec();
}

QPair<qint64, QString> FTPSocket::formatFileSize(const qint64  &filesizebyte, int precision)
{
    //qInfo()<<"filesizebyte "<<filesizebyte;

    QPair<qint64, QString> sizePair;
    if(filesizebyte == 0)
    {
        sizePair.first = 0;
        sizePair.second = "0B";
        return sizePair;
    }

    double sizeAsDouble = filesizebyte;
    QStringList measures = {"B","KB", "MB","GB","TB","PB", "EB","ZB", "YB"};

    int index = 0;
    while (sizeAsDouble >= 1024.0) {
        index++;
        sizeAsDouble /= 1024.0;
        //qInfo()<<"sizeAsDouble"<<sizeAsDouble<<index;
    }
    QString fileSize = QString::number(sizeAsDouble,'f', precision) + measures[index];
    //qInfo()<<"fileSize"<<fileSize;
    sizePair.first = static_cast<qint64>(filesizebyte);
    sizePair.second = fileSize;
    return  sizePair;
}

void FTPSocket::writeCommand(const QByteArray &comm, bool synchronization )
{
    qInfo()<<"开始comm"<<comm;
    //m_FtpServerSocket->write(comm);

    emit sigFtpSocketWrite(comm);
    if(synchronization)
    {
        QEventLoop loop;
        connect(this, &FTPSocket::sigFTPCommandFinish, &loop, &QEventLoop::quit);

        //3秒时间以后退出去
        QTimer::singleShot(3000, &loop, &QEventLoop::quit);
        loop.exec();
    }
    //qInfo()<<"结束comm";

}

void FTPSocket::writeCommand(const QString &comm, bool synchronization )
{
    QString strComm = comm;
    QByteArray commArray   = strComm.toUtf8();
    //    qInfo()<<"commArray = "<<commArray;
    writeCommand(commArray,synchronization);
}

QStringList FTPSocket::listFtpFile()
{
    qInfo()<<"是否登录成功"<<m_loginSuccess;
    m_ftpEnumStatus = FTPSocket::FTP_LISTFile;

    if(m_loginSuccess)
    {
        m_reconnections = 0;

        pasvMode();

        writeCommand(QString("NLST\r\n"));

        QEventLoop loop;

        connect(this, &FTPSocket::sigFTPListFileFinish,  &loop, &QEventLoop::quit);

        loop.exec();
        qInfo()<<"loop list end";
        closeFTPConnect();
        return m_ftpFileList;
    }
    else
    {

        //如果主机名不为空，说明已经登录过， 启动重连模式
        if(!m_hostName.isEmpty())
        {

            //最多三次机会重连，三次还是失败，结束返回
            if(m_reconnections > 3)
            {
                //发出重连失败信号
                emit sigFTPReconnectFaile();
                return QStringList("");
            }

            m_reconnections++;

            //开始重连,只有登录状态为false时才调用，这里做这个判断，是由于listFtpFile调用太快，可能后面已经登录成功有延时
            if(!m_loginSuccess)
            {
                connectFtpServer(m_hostName, m_hostPort);
                //进行阻塞
                QEventLoop loop;

                connect(this, &FTPSocket::sigFTPConnectFinish, &loop, &QEventLoop::quit);
                loop.exec();
            }

            //递归调用
            return listFtpFile();
        }
    }


    return m_ftpFileList;
}

qint64 FTPSocket::FtpFileSize(const QString &fileName)
{
    pasvMode();
    writeCommand(QString("SIZE %0\r\n").arg(fileName));

    qInfo()<<"FtpFileSize"<<m_dwFileSizePair.second;
    return m_dwFileSizePair.first;
}

void FTPSocket::FtpCWD(QString dirName)
{
    writeCommand(QString("CWD %0\r\n").arg(dirName));
    // FtpPWD();
}

void FTPSocket::FtpPWD()
{
    writeCommand(QString("PWD \r\n"));
}

void FTPSocket::closeFTPConnect(bool synchronization)
{
    qInfo()<<"closeFTPConnect, isLoginSuccess"<<m_loginSuccess;
    //如果没有登陆，调用直接返回，不进行任何操作
    if(!m_loginSuccess)
    {
        return;
    }
    if(m_FtpServerSocket){
        //qInfo()<<"当前的线程是"<<QThread::currentThread();
        m_loginSuccess = false;
        writeCommand(QByteArray("QUIT\r\n"),synchronization);
    }


}

void FTPSocket::setFtpMode(FTPSocket::FTPMODE ftpmode)
{
    m_ftpEnumMode = ftpmode;
}

void FTPSocket::setFtpHostName(QString hostName)
{
    m_hostName = hostName;
}

void FTPSocket::setFtpDownFileName(const QString &downFileName)
{
    m_dwfileName = downFileName;
}

void FTPSocket::setFtpsegmentedDownloadPoint(QPair<qint64, qint64> segmentedDownloadPoin)
{
    m_segmentedDownloadPoint = segmentedDownloadPoin;
}

void FTPSocket::setFtpDwonFileSize(qint64 filesize)
{
    m_dwFileSizePair.first = filesize;
}


void FTPSocket::ftpStartSegmentedDownload(const QString &ftpHost, const QByteArray & userName, const QByteArray & passwd, const QString & cdDir,const QString &downfileName)
{
    if(m_SegmentedDownload == nullptr)
    {
        m_SegmentedDownload = new SegmentedDownload;
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownloadProgress,this, &FTPSocket::sigDownloadProgress);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownFinish, this, &FTPSocket::sigFTPDownloadFinish);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownloadSpeedUpdate, this, &FTPSocket::sigDownloadSpeedUpdate);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownloadTimeUpdate, this, &FTPSocket::sigDownloadTimeUpdate);
        connect(m_SegmentedDownload, &SegmentedDownload::sigCloseFtpConnect, this, &FTPSocket::sigFTPCloseConnect);
		connect(m_SegmentedDownload, &SegmentedDownload::sigDownFileFaile, this, &FTPSocket::sigDownFileFaile);
		connect(m_SegmentedDownload, &SegmentedDownload::sigDownStreamFileFinish, this, &FTPSocket::sigFTPStreamFileFinish);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownLoadTimeout, this, &FTPSocket::sigDownloadTimeout);
    }
    else{
        //    return;
    }

    if(m_ftpEnumMode == FTPSocket::MODE_ThreadStream)
    {
        m_SegmentedDownload->setFtpMode(SegmentedDownload::MODE_Stream);
    }
    else
    {
        m_SegmentedDownload->setFtpMode(SegmentedDownload::MODE_FILE);
    }


    m_cdDir = cdDir;
    //为多线程做好准备
    m_SegmentedDownload->setDownFileName(downfileName);
    m_SegmentedDownload->setFtpHostName(ftpHost);
    m_SegmentedDownload->setFtpUserNamePass(userName, passwd);

    m_SegmentedDownload->setCDFileDirName(m_cdDir);

#ifdef MultiThreadRunDown
    //每个分段都是一个线程下载，是真正的多线程下载
    m_SegmentedDownload->startThreadDownload();
#else
    //在run中运行所有的线程分段下载，其实只开辟了一个多线程
    m_SegmentedDownload->setStackSize(1024*1024*1024);
    m_SegmentedDownload->start();
#endif
}

void FTPSocket::ftpStartSegmentedDownload(const QString &ftpHost, const QByteArray &userName, const QByteArray &passwd, const QString & cdDir,const QStringList &downfileName)
{
    if(!m_SegmentedDownload)
    {
        m_SegmentedDownload = new SegmentedDownload;

        connect(m_SegmentedDownload, &SegmentedDownload::sigDownloadProgress,this, &FTPSocket::sigDownloadProgress);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownFinish, this, &FTPSocket::sigFTPDownloadFinish);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownStreamFileFinish, this, &FTPSocket::sigFTPStreamFileFinish);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownloadSpeedUpdate, this, &FTPSocket::sigDownloadSpeedUpdate);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownloadTimeUpdate, this, &FTPSocket::sigDownloadTimeUpdate);
        connect(m_SegmentedDownload, &SegmentedDownload::sigCloseFtpConnect, this, &FTPSocket::sigFTPCloseConnect);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownFileFaile, this, &FTPSocket::sigDownFileFaile);
        connect(m_SegmentedDownload, &SegmentedDownload::sigDownLoadTimeout, this, &FTPSocket::sigDownloadTimeout);
    }
    else
    {
        // return;
    }

    if(m_ftpEnumMode == FTPSocket::MODE_ThreadStream)
    {
        m_SegmentedDownload->setFtpMode(SegmentedDownload::MODE_Stream);
    }
    else
    {
        m_SegmentedDownload->setFtpMode(SegmentedDownload::MODE_FILE);
    }

    //设置切换ftp的目录的名称
    m_cdDir = cdDir;

    //为多线程做好准备
    m_SegmentedDownload->setDownFileName(downfileName);
    m_SegmentedDownload->setFtpHostName(ftpHost);
    m_SegmentedDownload->setFtpUserNamePass(userName, passwd);
    m_SegmentedDownload->setCDFileDirName(m_cdDir);
#ifdef MultiThreadRunDown
    //每个分段都是一个线程下载，是真正的多线程下载
    m_SegmentedDownload->startThreadDownload();
#else
    //在run中运行所有的线程分段下载，其实只开辟了一个多线程
    m_SegmentedDownload->setStackSize(1024*1024*1024);
    m_SegmentedDownload->start();
#endif

}

void FTPSocket::ftpCloseSegmentedDownload()
{
    if(m_SegmentedDownload)
    {
        m_SegmentedDownload->closeResumeFile();
    }
}

void FTPSocket::startSegmentedDownload(QString downfileName, qint64 downFileSize, QByteArray downData,  QString  cdDir,  qint64 SegmenteddownPoint, QPair<qint64, qint64> segmentedDownloadPoint, QThread *thread)
{

    if(thread != nullptr && thread != QThread::currentThread())
    {
        return;
    }

    m_currentThread = QThread::currentThread();

    m_cdDir = cdDir;
    static int count = 0;
    count++;
    qInfo() << "Thread" << count<<thread;
    qInfo() << "current Thread" << count<< QThread::currentThread();
    //QMutexLocker locker(&m_mutex);
    setFtpDwonFileSize(downFileSize);

    //不为空就切换ftp目录
    if(!m_cdDir.isEmpty())
    {
        FtpCWD(m_cdDir);
    }

    writeCommand(QByteArray("TYPE I\r\n"), false);
    //设置为分段下载
    m_ftpEnumStatus = FTPSocket::FTP_SegmentedDownload;

    //记录当前的已经下载好的分段小文件的大小，断点续传就有，第一次下载
	m_fileCacheData = downData;
    //设置分段下载点
    m_segmentedDownloadPoint = segmentedDownloadPoint;
    //    qInfo()<<"当前线程= "<<QThread::currentThread();
    //    qInfo()<<"m_segmentedDownloadPoint = "<<m_segmentedDownloadPoint.first;
    //    qInfo()<<"segmentedDownloadPoint = "<<segmentedDownloadPoint.first;
    //    qInfo()<<"m_segmentedDownloadPoint = "<<m_segmentedDownloadPoint.second;
    //    qInfo()<<"segmentedDownloadPoint = "<<segmentedDownloadPoint.second;
    //设置偏移量,分段下载的开始点
    if(SegmenteddownPoint != 0)
    {
        writeCommand(QString("REST %1\r\n").arg(SegmenteddownPoint));
    }

    pasvMode();
    //开始下载
    writeCommand(QString("RETR %1\r\n").arg(downfileName));
}

void FTPSocket::startSegmentedDownload()
{
    qInfo()<<"startSegmentedDownload当前线程是."<<QThread::currentThread();
    qInfo()<<"准备连接fpt服务.";
    connectFtpServer(m_hostName);
    QTime timer;
    qInfo()<<"启动时间循环等待.";
    timer.start();
    QEventLoop loop;
    connect(this, &FTPSocket::sigFTPConnectFinish, &loop, &QEventLoop::quit);
    loop.exec();
    qInfo()<<"启动时间循环等待结束，耗时:"<<timer.elapsed()/1000<<"S";
    //QMutexLocker locker(&m_mutex);
    if(!m_cdDir.isEmpty())
    {
        FtpCWD(m_cdDir);
    }

    //设置为分段下载
    m_ftpEnumStatus = FTPSocket::FTP_SegmentedDownload;
    //设置分段下载点
    writeCommand(QByteArray("TYPE I\r\n"), false);

    qInfo()<<"m_segmentedDownloadPoint = "<<m_segmentedDownloadPoint;
    //设置偏移量,分段下载的开始点
    if(m_segmentedDownloadPoint.first != 0)
    {
        writeCommand(QString("REST %1\r\n").arg(m_segmentedDownloadPoint.first));
    }

    pasvMode();
    qInfo()<<"开始下载的文件Name ="<<m_dwfileName;
    //开始下载
    writeCommand(QString("RETR %1\r\n").arg(m_dwfileName));
}

int FTPSocket::splitPasvIpPort(const QString & PASVPassiveStr)
{
    int ip = 0;
    QString str = PASVPassiveStr;

    if(str.contains("("))
    {
        QStringList strlist = str.split("(");

        if(strlist.length() >= 2)
        {
            strlist = strlist.at(1).split(")");
            QString ipString = strlist.at(0);
            qInfo()<<"ipString"<<ipString;
            strlist = ipString.split(",");
            ip = strlist.at(4).toInt();
            int ip2 = strlist.at(5).toInt();

            ip = ip*256+ip2;
        }

    }


    return ip;
}

void FTPSocket::onReadData()
{
    QByteArray array = m_FtpServerSocket->readAll();
    qInfo()<<"ftpServer 读取到的数据"<<array;

    if(array.contains("234 Proceed with negotiation"))
    {
        emit this->sigFTPCommandFinish();

        QTimer::singleShot(DELAY_TIME, this, [=]{
            writeCommand(QString("AUTH TLSv1.2 \r\n"));
        });
    }
    //切换模式成功，开始监听ftp返回过来的数据端口
    else if(array.contains("227 Entering Passive Mode"))
    {
        //命令完成后触发这个信号，阻塞同步解除
        emit this->sigFTPCommandFinish();
        //splitPasvIpPort计算ftp服务器返回过来的数据端口
        QTimer::singleShot(DELAY_TIME, this, [=]{
            qInfo()<<"准备打通数据duan端口";
            listenFtpDataPort(static_cast<quint16>(splitPasvIpPort(array)));
        });

    }
    //输入账号后跳转到此，输入密码登录
    else if(array.contains("331 Please specify the password") || array.contains("331 Password required") )
    {
        //命令完成后触发这个信号，阻塞同步解除
        emit this->sigFTPCommandFinish();

        qInfo()<<"输入密码"<<m_ftpPasswd;
        qInfo()<<"当前的线程是A"<<QThread::currentThread();
        QTimer::singleShot(DELAY_TIME, this, [=]{
            qInfo()<<"当前的线程是B"<<QThread::currentThread();
            writeCommand(m_ftpPasswd);
        });

        //m_FtpServerSocket->write(m_ftpPasswd);
    }

    //登录成功后状态码进来进来，切换被动模式
    else if(array.contains("230 User") || array.contains("230 Login successful"))
    {
        //命令完成后触发这个信号，阻塞同步解除

        emit this->sigFTPCommandFinish();
        QTimer::singleShot(DELAY_TIME, this, [=] {
            qInfo() << "登录成功";
            m_loginSuccess = true;
            //发射连接完成的信号，至此，ftp连接成功，可以进行各种操作
            emit sigFTPConnectFinish();
        });

    }

    //切换目录成功
    else if(array.contains("250 CWD"))
    {
        emit this->sigFTPCommandFinish();
        QTimer::singleShot(DELAY_TIME, this, [=]{

            emit sigFTPCWDSuccess();
        });
    }
    //打印当前目录成功
    else if(array.contains("257"))
    {
        int indexA = 5;
        int indexB = array.indexOf("is");
        QString currentDir = array.mid(indexA,indexB - indexA - 2 );
        qInfo()<<"当前截取到的目录 = "<<currentDir;
        emit this->sigFTPCommandFinish();

        QTimer::singleShot(DELAY_TIME, this, [=]{
            qInfo()<<"当前的目录 = "<<currentDir;
            emit sigFTPPWDFinish(currentDir);
        });
    }
    //超时登出，需要重新登录
    else if(array.contains("421 Timeout"))
    {
        //命令完成后触发这个信号，阻塞同步解除
        emit this->sigFTPCommandFinish();
        //pasvMode();
        // m_loginSuccess = false;
        QTimer::singleShot(DELAY_TIME, this, [=]{
            closeFTPConnect();
        });

    }

    //获取文件大小
    else if(array.contains("213") )
    {
        QString filesize(array);
        filesize = filesize.simplified();
        filesize = filesize.mid(4);
        qInfo()<<"转发的fileSize"<<filesize<<filesize.toLongLong()<<filesize.toInt();
        m_dwFileSizePair =  formatFileSize(filesize.toLongLong());
        //命令完成后触发这个信号，阻塞同步解除
        emit this->sigFTPCommandFinish();
    }

    //下载文件传输完成了
    else if(array.contains("226 Transfer complete"))
    {
        //命令完成后触发这个信号，阻塞同步解除
        emit this->sigFTPCommandFinish();
        QTimer::singleShot(DELAY_TIME, this, [=]{
            m_isFileDownFinish = true;
            if(m_ftpEnumMode != FTPMODE::MODE_ThreadStream)
            {
                saveDownloadFile();
            }

        });
    }
    // 退出ftp连接
    else if (array.contains("221 Goodbye") || array.contains("221 goodbye"))
    {
        emit this->sigFTPCommandFinish();
        QTimer::singleShot(DELAY_TIME, this, [=]{
            if (m_FtpServerSocket)
            {
                m_FtpServerSocket->close();
                m_FtpServerSocket->deleteLater();
                m_FtpServerSocket = nullptr;
            }

            if (m_FtpDataSocket)
            {
                m_FtpDataSocket->close();

                m_FtpDataSocket->deleteLater();

                m_FtpDataSocket = nullptr;
            }
            //延迟触发关闭信号
            emit sigFTPCloseConnect();
        });
    }
    else if(array.contains("500 OOPS:"))
    {
        //         writeCommand(QByteArray("QUIT \r\n"));
        //         connectFtpServer(m_hostName);
    }

    //ftp 下载文件重置下载源位置
    else if (array.contains("350 Restart position accepted"))
    {
        qInfo()<<"进来重置";
        emit this->sigFTPCommandFinish();
    }

    else if(array.contains("150 Opening BINARY mode data connection") || array.contains("150"))
    {
        emit this->sigFTPCommandFinish();
    }
    //连接登录用户失败
    else if(array.contains("530 User cannot login"))
    {
        emit this->sigFTPCommandFinish();
        QTimer::singleShot(DELAY_TIME, this, [=]{
            //延迟触发关闭信号
            qInfo()<<"530 ftr connect faile";
            emit sigFTRConnectFaile();
        });
    }

    //切换目录失败
    else  if(array.contains("550 Failed to change directory"))
    {
        emit this->sigFTPCommandFinish();
    }

    else if(array.contains("550 The semaphore timeout period has expired"))
    {

        //发送连接中断信号
        emit this->sigFtpConnectInterruption();
        //先关闭，然后重新连接
        this->closeFTPConnect();

        this->connectFtpServer(m_ftpUserName);
    }
}

void FTPSocket::onFtpDataRead()
{
  //  QMutexLocker locker(&m_mutex);
    QByteArray array = m_FtpDataSocket->readAll();

    //  qInfo()<<"FTPdataarray读取到的数据"<<array;
    if(array.isEmpty())
    {
        //   qInfo()<<"请求到的ftp数据为空退出";
        return;
    }

    QString ftpFileStr;
    QPair<qint64, QString> pairFileSize;
    switch (static_cast<int>(m_ftpEnumStatus))
    {
    //获取文件的列表
    case FTPSocket::FTP_LISTFile:
        qInfo()<<"FTPList进来";
        ftpFileStr = QString(array);
        m_ftpFileList = ftpFileStr.split("\r\n");
        m_ftpFileList.removeAll("");

        emit sigFTPListFileFinish();
        break;
        //采用单线程下载
    case FTPSocket::FTP_DOWNFile:

        m_fileCacheData += array;

        //qInfo()<<"下载的文件数据"<<array;
        pairFileSize = formatFileSize(m_fileCacheOffset+m_fileCacheData.length());
        //qInfo()<<"文件大小"<<pairFileSize.first;

        //qInfo()<<"原始文件大小"<<m_dwFileSizePair.second;
        if(m_ftpEnumMode == FTPSocket::MODE_FILE)
        {
            emit sigDownloadProgress(pairFileSize.first, m_dwFileSizePair.first);
        }

        break;

        //采用分段下载模式
    case FTPSocket::FTP_SegmentedDownload:

		if (m_currentThread != QThread::currentThread())
		{
			return;
		}

//		qInfo() << "m_currentThread =" << m_currentThread;
		qInfo() << "QThread::currentThread()" << QThread::currentThread();

        m_fileCacheData += array;

        qint64 fileCachasize = m_segmentedDownloadPoint.first+m_fileCacheData.size();
        qInfo()<<"A----"<<fileCachasize;
        //记录每个分段下载的总长度
        //pairFileSize = formatFileSize(m_segmentedDownloadPoint.first + fileCachasize);
        //        qInfo()<<"当前线程= "<<QThread::currentThread();
       
        //          qInfo()<<"m_dwFileSizePair = "<<m_dwFileSizePair;
        //              qInfo()<<"m_fileCacheSize = "<<m_fileCacheSize;
        //qInfo() << "pairFileSize = " <<pairFileSize;
		//qInfo() << "pairFileSize = " << pairFileSize.first;
		qInfo() << "segmentedDownloadPoint = " << m_segmentedDownloadPoint.second;
       
		//QTimer::singleShot(1000, this, [=] {
			//qInfo() << "1s到进来，发射信号";
		emit sigFTPSegmentedDownloadData(m_fileCacheData, fileCachasize);
		//});
        if (fileCachasize >= m_segmentedDownloadPoint.second ) //&& m_segmentedDownloadPoint.second != m_dwFileSizePair.first
        {
            //            qInfo()<<"进来准备终止";
            aborFtpDataConnect();
            closeFTPConnect();
        }

        break;
    }
}

void FTPSocket::displayError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
}

void FTPSocket::onFtpConnectFinish()
{
    // qInfo()<<"当前线程是"<<QThread::currentThread();
    //连接数据成功，写入用户名登录
    qInfo()<<"连接成功,请输入用户名";
    //writeCommand(QByteArray("AUTH TLS\r\n"));
    writeCommand(m_ftpUserName);
    //m_FtpServerSocket->write(m_ftpUserName);
}
//-----------------------下面是分段下载的区域 -------------------------------------

SegmentedDownload::SegmentedDownload(QObject *parent): QObject(parent)
{
    //注册类型
    qRegisterMetaType<QPair<qint64, qint64>>("QPair<qint64, qint64>");

    m_downUpdateTimer.setInterval(1000);
    m_downUpdateTimer.setSingleShot(false);


    connect(&m_downUpdateTimer, &QTimer::timeout, this, &SegmentedDownload::onUpdateDownloadSpeed);
    connect(&m_downTimeoutTimer, &QTimer::timeout, this, &SegmentedDownload::onDownloadTimeout);  //下载超时触发

    m_downTimeoutTimer.setInterval(m_downTimeout);
    m_downTimeoutTimer.setSingleShot(true);
}

SegmentedDownload::~SegmentedDownload()
{
    qInfo()<<"SegmentedDownload 析构进来";
    m_isDestory = true;
    initAllListData();

}

void SegmentedDownload::setFtpMode(SegmentedDownload::FTPMODE ftpModel)
{
    m_ftpModel = ftpModel;
}

void SegmentedDownload::setMaxThreadNum(int threadNum)
{
    m_maxThreadNum = threadNum;
    return;
}

void SegmentedDownload::setDownFileSize(const qint64 &fileSize)
{
    m_downFileSize = fileSize;
    return;
}

void SegmentedDownload::setDownFileName(const QString & fileName)
{
    m_downFileName = fileName;
    return;
}

void SegmentedDownload::setCDFileDirName(const QString &dirName)
{
    m_cdDirName = dirName;
    return;
}

void SegmentedDownload::setDownFileName(const QStringList &fileNameList)
{
    m_downFileNameList = fileNameList;
    return;
}

void SegmentedDownload::startThreadDownload()
{
    int i = 0;
    //!在下载前初始化所有的变量
    initAllListData();

    //获取下载文件的大小，必须调用，不然下面的所有程序走不通
    getDownFileSize();
    qInfo()<<"下载完毕-=-=-="<<m_downFileSize;

    if(m_isDestory)
    {
        return;
    }
    //	m_downFileSize = 6742528;
    //如果下载文件是0则直接返回
    if(m_downFileSize == 0 && m_downFileNameList.isEmpty())
    {
        emit sigDownFileFaile(tr("Remote file reading failed"));
        qWarning("Segmented download file size is 0");

        return;
    }

    //如果外面提供的分段文件为空，则设置程序下载分段块
    if(m_downFileNameList.isEmpty())
    {
        //根据线程数来平均分段
        qint64 downPoint =  m_downFileSize/m_maxThreadNum;

        //下载块
        m_DownBlock = downPoint;
    }
	
	//线程数大于5时
	if (m_maxThreadNum >= 5)
	{
		m_downTimeout = 30000;  //设置单独超时时间为30S
	}
	
	
    //读取断点续传的文件，如果ftpmodel为MODE_Stream则不进行分段下载，也不进行文件存储
    readResumeFile();

    //记录下载总共花销了多少时间，测试使用而已
    m_downTime.start();

    //计算文件下载的网速时间,测速和剩余时间专用
    m_downElapsedTime.start();

    //更新下载时间和网速到UI界面，每隔1秒钟触发一次
    m_downUpdateTimer.start();

    qInfo()<<"m_maxThreadNum = "<<m_maxThreadNum;

    //开始循环线程分段下载
    for(i = 0; i < m_maxThreadNum; i++)
    {
#ifdef MultiThreadRunDown
        FTPSocket * ftpSocket = new FTPSocket;
        QThread * thread = new QThread;

        //qInfo()<<"当前的线程是"<<thread->currentThread();

        //设置线程的堆栈大小
        //thread->setStackSize(1024*1024*1024);
        //进入当前的ftpsocket
        m_FTPSocketList.append(ftpSocket);
        //记录所有的thread
        m_ListThread.append(thread);

        //启动线程后触发
        connect(thread, &QThread::started, this, &SegmentedDownload::onStartFtpConnect,Qt::QueuedConnection);

        //进行ftp的线程连接
        connect(this, QOverload<QString, quint16, QByteArray, QByteArray>::of(&SegmentedDownload::sigStartFtpConnect), ftpSocket,
                QOverload<QString, quint16, QByteArray, QByteArray>::of(&FTPSocket::connectFtpServer),Qt::QueuedConnection);

        //ftp客户端连接成功后触发，准备ftp下载前的准备工作，进行数据的 校验和设置等
        connect(ftpSocket, &FTPSocket::sigFTPConnectFinish,this,&SegmentedDownload::onReadyDownFile);

        //下载程序触发，调用ftp的下载功能，进行分段下载
        connect(this, QOverload<QString, qint64,QByteArray, QString, qint64, QPair<qint64, qint64 >,QThread*>::of(&SegmentedDownload::sigDownFile),
                ftpSocket,QOverload<QString, qint64, QByteArray, QString , qint64, QPair<qint64, qint64>,QThread*>::of(&FTPSocket::startSegmentedDownload), Qt::QueuedConnection);

        connect(this, &SegmentedDownload::sigaborFtpConnect, ftpSocket, &FTPSocket::aborFtpAllConnect);
        //分段下载触发，每当有数据过来则触发
        connect(ftpSocket, &FTPSocket::sigFTPSegmentedDownloadData, this,  &SegmentedDownload::onDownloadDataUpdate);

        //        //ftp关闭进来，也就是分段下载的小文件已经下载完成后触发
        connect(ftpSocket, &FTPSocket::sigFTPCloseConnect, this, &SegmentedDownload::onFtpClostConnect,Qt::QueuedConnection);

        ftpSocket->moveToThread(thread);
        thread->start();

#else
        qInfo()<<"当前线程是 === i"<<i<<QThread::currentThread();
        FTPSocket * ftpSocket = new FTPSocket;
        qInfo() << "ftpSocket" << ftpSocket;
        ftpSocket->setFtpHostName(ftpHost);
        qInfo() << "FTPSocket = " << ftpSocket;

        ftpSocket->setFtpDwonFileSize(m_downFileSize);
        ftpSocket->setFtpUserNamePasswd(ftpUserName, ftpUserPasswd);
        ftpSocket->setFtpsegmentedDownloadPoint(m_listDownSegmentedPoint.at(i));
        ftpSocket->setFtpDownFileName(m_downFileName);

        //qInfo()<<"当前线程stacksize = "<<ftpSocketm_listDownSegmentedInterval        //qInfo()<<"设置以后的stacksize = "<<ftpSocket->stackSize();
        QMap<FTPSocket *, QByteArray> ftpMapSocket;

        //往后追加
        ftpMapSocket[ftpSocket] = "";
        m_listFtpArray.append(ftpMapSocket);



        //        //移动到线程中去
        //        ftpSocket->moveToThread(thread);
        //        //保存每一个线程，用来结束对应的线程
        //        m_thread.append(thread);

        //多线程启动连接
        //   QObject::connect(this, QOverload<QString, quint16, QByteArray, QByteArray>::of(&SegmentedDownload::sigStartFtpConnect), ftpSocket,
        //  QOverload<QString, quint16, QByteArray, QByteArray>::of(&FTPSocket::connectFtpServer),Qt::DirectConnection);

        // connect(ftpSocket, QOverload<>::of(&FTPSocket::sigFTPConnectFinish), ftpSocket, QOverload<>::of(&FTPSocket::startSegmentedDownload),Qt::DirectConnection);

        // connect(this, QOverload<QString>::of(&SegmentedDownload::sigDownFile),
        //   ftpSocket,QOverload<QString>::of(&FTPSocket::startSegmentedDownload));

        connect(ftpSocket, &FTPSocket::sigFTPSegmentedDownloadData, this, [=](QByteArray downData,  qint64 bytesReceived){
            QMutexLocker locker(&m_mutex);
            m_ProgresslistByte[i] = bytesReceived;
            m_listFtpData[i] = downData;
            if(m_ProgresslistByte[i] >= m_listDownSegmentedPoint[i].second && !m_downFinish[i])
            {
                m_downFinish[i] = true;
                m_listFtpData[i] =  static_cast<int>(m_listDownSegmentedPoint[i].second - m_listDownSegmentedPoint[i].first));

            }
        });

        connect(ftpSocket, &FTPSm_listDownSegmentedIntervalt, this, [=]{m_listDownSegmentedIntervalr locker(&m_mutex);
            bool isAllFinish = true;
            for(auto isFinish:m_downFinish)
            {
                //如果有一个没有完成这位false
                if(!isFinish)
                {
                    isAllFinish = false;
                    break;
                }
            }

            if(isAllFinish && m_isAllFinish != 3)
            {
                qInfo()<<"下载线程完成，准备写文件";
                m_isAllFinish = 3;
                //设置下载的位置
                QString dirPath = HelpClass::getCurrentTempDataDir();
                dirPath = dirPath+"/downFile";

                QDir dir(dirPath);
                if(!dir.exists())
                {
                    dir.mkdir(dirPath);
                }

                QFileInfo fileInfo(m_downFileName);


                QFileInfo info;
                info.setFile(dir, m_downFileName);
                QFile * file = new QFile(info.filePath());

                if (!file->open(QIODevice::WriteOnly))
                {
                    qWarning("file open faile");
                    return;
                }


                for(int i = 0; i < m_listFtpData.length(); i++)
                {
                    auto mapFtp = m_listFtpData[i];

                    file->write(mapFtp);
                    file->flush();
                }

                file->close();
                delete file;

                qInfo() << "计时结束,总共耗时" << time.elapsed() / 1000 << "s";
            }
        });
        //      connect(ftpSocket, &FTPSocket::sigFTPSegmentedDownloadEnd, this ,&SegmentedDownload::onSegmentedDownloadFinish);

        //connect(this, &SegmentedDownload::sigCloseFtpConnect, ftpSocket, &FTPSocket::closeFTPConnect);

        ftpSocket->startSegmentedDownload();
        //  thread->start();

        //启动ftp连接
        // emit this->sigStartFtpConnect(ftpHost, port, ftpUserName, ftpUserPasswd);
        // m_loop.exec();
#endif
    }

    //! 循环完毕后，说明下载已经开始启动，开始启动超时计时
  //  m_downTimeoutTimer.start();
}

void SegmentedDownload::setFtpHostName(const QString & hostName)
{
    m_hostName = hostName;
}

void SegmentedDownload::setFtpUserNamePass(const QByteArray &userName, const QByteArray &passwd)
{
    m_userName = userName;
    m_userPass = passwd;
}

void SegmentedDownload::closeResumeFile()
{
    initAllListData();
    //    for(auto &thread : m_ListThread)
    //    {
    //        if(thread->isRunning())
    //        {
    //            thread->quit();
    //            thread->wait();
    //        }
    //    }
    //    //删除所有的ftp客户端socket
    //    qDeleteAll(m_FTPSocketList);
    //    m_FTPSocketList.clear();

    //    //定义了每一个分段都是一个线程则执行下面的步骤
    //#ifdef MultiThreadRunDown
    //    //删除所有的线程
    //    qDeleteAll(m_ListThread);
    //    m_ListThread.clear();
    //#endif
    //    for(auto file:m_fileResumeList)
    //    {
    //        if(file->isOpen())
    //        {
    //            file->close();
    //        }
    //    }

    //    qDeleteAll(m_fileResumeList);

    //    m_fileResumeList.clear();
}

void SegmentedDownload::getDownFileSize()
{

    QEventLoop eventLoop;
    FTPSocket ftpSocket;

    //连接到ftp服务器
    ftpSocket.connectFtpServer(m_hostName, 21, m_userName, m_userPass);

    //同步等待
    connect(&ftpSocket, &FTPSocket::sigFTPConnectFinish, &eventLoop, &QEventLoop::quit);

    eventLoop.exec();

	if (m_isDestory)
	{
		return;
	}
    //如果切换目录的名称为空，则不进行目录切换，为了以防万一进行目录切换
    if(!m_cdDirName.isEmpty())
    {
        ftpSocket.FtpCWD(m_cdDirName);
    }

	if (m_isDestory)
	{
		return;
	}

    //为空说明下载的单文件，由程序来分块下载，否则就是外面已经分好块了
    if(m_downFileNameList.isEmpty())
    {
        //获取ftp文件的大小
        m_downFileSize =  ftpSocket.FtpFileSize(m_downFileName);
    }
    else {
        //直接获取每个分块文件的大小
        for(int i = 0; i < m_downFileNameList.length(); i++)
        {
            m_downFileSizeList.append(ftpSocket.FtpFileSize(m_downFileNameList.at(i)));
            //计算总的文件大小
            m_downFileSize += m_downFileSizeList[i];
        }
    }

	if (m_isDestory)
	{
		return;
	}

    ftpSocket.closeFTPConnect();


}

QDir SegmentedDownload::saveDownFileDir()
{
    QString dirPath = HelpClass::getCurrentTempDataDir();
    dirPath = dirPath+"/downFile";

    QDir dir(dirPath);
    if(!dir.exists())
    {
        dir.mkdir(dirPath);
    }

    return dir;
}

bool SegmentedDownload::readResumeFile()
{
    QDir dir = saveDownFileDir();
    m_downFinish.clear();
    //如果非空，说明文件分区由外界已经做好，只需要下载对应的文件，然后整合及可
    if(!m_downFileNameList.isEmpty())
    {
        //设置当前最大的线程数为外界分割的文件数量
        m_maxThreadNum = m_downFileNameList.length();

        //截取下载文件的后缀
        QStringList strDownFile = m_downFileNameList[0].split("exe");
        //保存下载文件的名称
        m_downFileName = strDownFile[0]+"exe";

        for(int i = 0; i < m_maxThreadNum; i++)
        {
            //进度的相关初始化
            m_ProgresslistByte.append(0);
            //下载完成的标记初始化，统一为false，true为下载成功
            m_downFinish.append(false);
            //分段下载点的声明
            QPair<qint64, qint64> segmentedDownPoint;
            //赋值分段开始点
            segmentedDownPoint.first = 0;  //永远从0开始到分区文件的结束，这个是由外界分好的par文件
            segmentedDownPoint.second = m_downFileSizeList.at(i);  //结束点是每个文件的大小
            m_listDownSegmentedInterval.append(segmentedDownPoint);

            //如果是文件下载模式，则进行下载文件前的准备，进行预下载处理
            if(m_ftpModel == FTPMODE::MODE_FILE)
            {
                //下载文件准备中
                QFileInfo fileInfo;

                fileInfo.setFile(dir, m_downFileNameList.at(i));

                //qInfo()<<"down file path = "<<fileInfo.filePath();
                //如果文件路径存在，则说明有缓存文件，需要进行断点续传操作
                if(QFile::exists(fileInfo.filePath()))
                {
                    if(m_downElapsedTimeCacle == 0)
                    {
                        //读取指针的时间
                        QMap<QString ,QString>mapString;
                        mapString["seekTime"] =QString::number(m_nTime);
                        QString filePath = HelpClass::getCurrentTempDataDir()+"/downFile";

                        HelpClass::isDirExist(filePath);
                        filePath = filePath+"/seek.ini";
                        mapString =  HelpClass::readSettingFile("Seek", filePath, false);
                        m_downElapsedTimeCacle = mapString["seekTime"].toInt();
                        mapString =  HelpClass::readSettingFile("Seek", filePath, false);
                        m_TotalTime = mapString["seekTotalTime"].toInt();
                    }

                    //读取未下载完的缓存数据
                    QFile * pfile = new QFile(fileInfo.filePath());
                    pfile->open(QIODevice::ReadOnly| QIODevice::WriteOnly);
                    QByteArray data = pfile->readAll();
                    pfile->close();
                    m_fileResumeList.append(pfile);
                    //赋值数据点
                    m_listFtpData.append(data);
                    //设置分段下载的开始点，并不是从0开始
                    m_SegmentedPoint.append(data.length());
                }
                else
                {
                    //不需要进行断点续传，所有数据初始化操作
                    QFile  * pfile = new QFile(fileInfo.filePath());

                    m_fileResumeList.append(pfile);
                    //ftp数据初始化
                    m_listFtpData.append("");

                    //赋值分段下载开始点，从每个文件的0处开始下载
                    m_SegmentedPoint.append(m_listDownSegmentedInterval[i].first);
                }
            }
            //流下载模式
            else {
                m_listFtpData.append("");

                //赋值分段开始点
                m_SegmentedPoint.append(m_listDownSegmentedInterval[i].first);
            }

        }

        return true;
    }
    //如果m_downFileNameList是空的则自动分裂文件的下载点，动用分区下载
    else{

        QString tempDownFileName;
        QString suffix ;

        if(m_ftpModel == FTPMODE::MODE_FILE)
        {
            //赋值当前的下载的文件信息
            QFileInfo info(m_downFileName);
            suffix = info.suffix();

            tempDownFileName = m_downFileName;
            tempDownFileName.remove(suffix);
            tempDownFileName+=PAR_FILE_SUFFIX;
        }

        for(int i = 0; i < m_maxThreadNum; i++)
        {
            //进度的相关初始化
            m_ProgresslistByte.append(0);
            //下载完成的标记初始化，统一为false
            m_downFinish.append(false);

            //分段下载点的声明
            QPair<qint64, qint64> segmentedDownPoint;
            //赋值分段开始点
            segmentedDownPoint.first = i* m_DownBlock;

            //如果第二个分段结束点是最后的线程数，直接赋值最后的最大值给最后分段点，避免分段不均匀的情况出来
            if(i+1 == m_maxThreadNum)
            {
                segmentedDownPoint.second = m_downFileSize;
            }
            else {
                segmentedDownPoint.second = (i+1)*m_DownBlock;
            }

            m_listDownSegmentedInterval.append(segmentedDownPoint);

            if(m_ftpModel == FTPMODE::MODE_FILE)
            {
                //设置即将要下载的文件名等信息
                QString tempParDownFile = tempDownFileName;
                QFileInfo fileInfo;
                tempParDownFile += QString::number(i);
                fileInfo.setFile(dir, tempParDownFile);

                if(QFile::exists(fileInfo.filePath()))
                {
                    QFile * pfile = new QFile(fileInfo.filePath());
                    pfile->open(QIODevice::ReadOnly| QIODevice::WriteOnly);
                    QByteArray data = pfile->readAll();
                    pfile->close();
                    m_fileResumeList.append(pfile);
                    //赋值数据点
                    m_listFtpData.append(data);
                    //赋值分段开始点
                    m_SegmentedPoint.append((i* m_DownBlock) + data.length());
                }
                else {

                    QFile  * pfile = new QFile(fileInfo.filePath());

                    m_fileResumeList.append(pfile);
                    m_listFtpData.append("");

                    //赋值分段开始点
                    m_SegmentedPoint.append(m_listDownSegmentedInterval[i].first);
                }
            }
            //流下载模式
            else {
                m_listFtpData.append("");

                //赋值分段开始点
                m_SegmentedPoint.append(m_listDownSegmentedInterval[i].first);
            }

        }

        return true;
    }

}

void SegmentedDownload::initAllListData()
{
    //强制关闭所有的ftp线程连接
    //emit sigaborFtpConnect();

    //如果请求ftp文件大小没关闭直接关闭

    //![清空下面所有存储的数据]
    m_listFtpData.clear();
    m_ProgresslistByte.clear();
    m_downFileSizeList.clear();
    m_SegmentedPoint.clear();
    m_downFinish.clear();
    m_listDownSegmentedInterval.clear();

    m_downTimeoutTimer.stop();
    m_downFileSize = 0;
    m_TotalTime = 0;
    m_nTime = 0;
    m_downElapsedTimeCacle = 0;
    m_DownBlock = 0;
    m_ftpCloseNum = 0;


    //如果有东西则删除
    if(!m_fileResumeList.isEmpty())
    {
        //关闭所有的文件句柄, 然后清空
        for(auto &file:m_fileResumeList)
        {
            if(file)
            {
                if(file->isOpen())
                {
                    file->close();
                }
                delete file;
                file = nullptr;
            }
        }
        m_fileResumeList.clear();
    }




    //如果线程指针非空则进来清空
    if(!m_ListThread.isEmpty())
    {
        for(auto &thread:m_ListThread)
        {
            if(thread)
            {
                if(thread->isRunning())
                {
                    thread->quit();
                    thread->wait();
                }
            }
        }
        qDeleteAll(m_ListThread);
        m_ListThread.clear();
    }

    //如果所有ftp客户端非空则进来清空, 先关闭所有的客户端，在关闭线程
    if (!m_FTPSocketList.isEmpty())
    {
        qDeleteAll(m_FTPSocketList);
        m_FTPSocketList.clear();
    }

}

//void SegmentedDownload::run()
//{
//    //启动线程下载
//    startThreadDownload();

//    exec();
//}

void SegmentedDownload::onReadyDownFile()
{
    QMutexLocker locker(&m_mutex);

    //判断是哪一个指针发送的信号
    if(FTPSocket * tempFtpSocket = static_cast<FTPSocket*>(sender()))
    {
        for(int i = 0; i < m_FTPSocketList.length(); i++)
        {
            if(m_FTPSocketList[i]== tempFtpSocket)
            {
                //是空的，说明下载文件有程序来分割下载块
                if(m_downFileNameList.isEmpty())
                {
                    emit sigDownFile(m_downFileName, m_downFileSize ,m_listFtpData[i],  m_cdDirName,  m_SegmentedPoint[i], m_listDownSegmentedInterval[i], m_ListThread[i]);
                }
                else{

                    emit sigDownFile(m_downFileNameList[i],m_downFileSizeList[i], m_listFtpData[i], m_cdDirName, m_SegmentedPoint[i], m_listDownSegmentedInterval[i], m_ListThread[i]);
                }
            }
        }
    }
}


void SegmentedDownload::onStartFtpConnect()
{
    emit this->sigStartFtpConnect(m_hostName , 21, m_userName, m_userPass);
}

void SegmentedDownload::onUpdateDownloadSpeed()
{
    //如果是写文件模式，则记录当前的seekTime的时间
    if(m_ftpModel == FTPMODE::MODE_FILE)
    {
        QMap<QString ,QString>mapString;
        mapString["seekTime"] =QString::number(m_nTime);
        QString filePath = HelpClass::getCurrentTempDataDir()+"/downFile";
        HelpClass::isDirExist(filePath);
        filePath = filePath+"/seek.ini";
        HelpClass::writeSettingFile(mapString, "Seek", filePath, false);
        mapString["seekTotalTime"] =QString::number(m_TotalTime);
        HelpClass::writeSettingFile(mapString, "Seek", filePath, false);
    }
    // 本次下载所用时间
    m_nTime -= m_TotalTime;
    //qInfo()<<"计算后的结果m_nTime ="<<m_nTime;

    // 下载速度
    double dBytesSpeed = (m_progressBytes * 1000.0) / m_nTime;
    double dSpeed = dBytesSpeed;

    //剩余时间
    qint64 leftBytes = (m_downFileSize - m_progressBytes);
    double dLeftTime = (leftBytes * 1.0) / dBytesSpeed;

    //更新下载实时网速
    QString speedInfo = HelpClass::speed(dSpeed);
    //qInfo()<<"当前网速"<<speedInfo;
    emit sigDownloadSpeedUpdate(speedInfo);

    //更新下载的剩余完成时间
    QString surplusTime = HelpClass::timeFormat(qCeil(dLeftTime));
    emit sigDownloadTimeUpdate(surplusTime);

    // 获取上一次的时间
    m_TotalTime = m_nTime;

}

void SegmentedDownload::onDownloadDataFinish()
{
    // QMutexLocker locker(&m_mutex);

    bool isAllFinish = true;
    for(auto isFinish:m_downFinish)
    {
        //如果有一个没有完成这位false
        if(!isFinish)
        {
            isAllFinish = false;
            break;
        }
    }

    ////    //退出当前线程
    //    for(auto thread:m_ListThread)
    //    {
    //        thread->quit();
    //        thread->wait();
    //        delete thread;
    //        thread = nullptr;
    //    }


    if(isAllFinish && m_isAllFinish != 3)
    {
        qInfo()<<"下载线程完成，准备写入文件.";
        m_downTimeoutTimer.stop();
        m_isAllFinish = 3;
        m_ftpCloseNum = 0;
        for(auto &thread:m_ListThread)
        {
            if(thread)
            {

                //断掉所有的信号和槽连接
                for(auto & ftpSocket:m_FTPSocketList)
                {
                    //启动线程后触发
                    disconnect(thread, &QThread::started, this, &SegmentedDownload::onStartFtpConnect);

                    //进行ftp的线程连接
                    disconnect(this, QOverload<QString, quint16, QByteArray, QByteArray>::of(&SegmentedDownload::sigStartFtpConnect), ftpSocket,
                               QOverload<QString, quint16, QByteArray, QByteArray>::of(&FTPSocket::connectFtpServer));

                    //ftp客户端连接成功后触发，准备ftp下载前的准备工作，进行数据的 校验和设置等
                    disconnect(ftpSocket, &FTPSocket::sigFTPConnectFinish,this,&SegmentedDownload::onReadyDownFile);

                    //下载程序触发，调用ftp的下载功能，进行分段下载
                    disconnect(this, QOverload<QString, qint64,QByteArray, QString, qint64, QPair<qint64, qint64 >,QThread*>::of(&SegmentedDownload::sigDownFile),
                               ftpSocket,QOverload<QString, qint64, QByteArray, QString, qint64, QPair<qint64, qint64>,QThread*>::of(&FTPSocket::startSegmentedDownload));

                    //分段下载触发，每当有数据过来则触发
                    disconnect(ftpSocket, &FTPSocket::sigFTPSegmentedDownloadData, this,  &SegmentedDownload::onDownloadDataUpdate);

                    //        //ftp关闭进来，也就是分段下载的小文件已经下载完成后触发
                    disconnect(ftpSocket, &FTPSocket::sigFTPCloseConnect, this, &SegmentedDownload::onFtpClostConnect);

                    disconnect(this, &SegmentedDownload::sigaborFtpConnect, ftpSocket, &FTPSocket::aborFtpAllConnect);
                }


                if(thread->isRunning())
                {
                    thread->quit();
                    thread->wait();
                }

            }
        }

        qDeleteAll(m_ListThread);
        m_ListThread.clear();

        //这里因为ftp已经全部关闭，不需要再次调用关闭，只需要全部删除并清空则可
        qDeleteAll(m_FTPSocketList);
        m_FTPSocketList.clear();

        m_downFinish.clear();

        //        for(auto ftpSocket:m_FTPSocketList)
        //        {
        //            ftpSocket->closeFTPConnect(false);
        //        }
        m_downUpdateTimer.stop();
        if(m_ftpModel == FTPMODE::MODE_FILE)
        {
            //设置下载的位置
            QDir dir = saveDownFileDir();

            QFileInfo fileInfo(m_downFileName);

            QFileInfo info;

            QString tempDownFileName = m_downFileName;
            //首先移除这个exe后缀
            tempDownFileName.remove(fileInfo.suffix());
            //追加指定的临时后缀
            tempDownFileName += DOWNLOAD_FILE_SUFFIX;

            //设置相关的文件信息
            info.setFile(dir, tempDownFileName);
            fileInfo.setFile(dir, m_downFileName);
            //设置下载的文件名称
            QFile * file = new QFile(info.filePath());

            if (!file->open(QIODevice::WriteOnly))
            {
                qWarning("file open faile");
                emit sigDownFileFaile(tr("exe更新程序正在被占用，请关闭后重新更新."));
                return;
            }

            for(int i = 0; i < m_listFtpData.length(); i++)
            {
                auto mapFtp = m_listFtpData[i];

                file->write(mapFtp);
                file->flush();
            }

            //下载完成关闭对应的文件操作
            file->close();

            //删除原有的下载文件
            if(QFile::exists(fileInfo.filePath()))
            {
                if(!QFile::remove(fileInfo.filePath()))
                {
                    qWarning("文件被占用，删除失败.");
                    emit sigDownFileFaile(tr("File Occupied, Failed to Open File"));
                    return;
                }
            }

            file->rename(fileInfo.filePath());

            delete file;

            //写入断点续传的文件,并删除
            for(auto file:m_fileResumeList)
            {
                //file->close();
                if (file->isOpen())
                {
                    file->close();
                }

                file->remove(file->fileName());
            }
            qDeleteAll(m_fileResumeList);
            m_fileResumeList.clear();
            QFileInfo  inifileInfo;
            inifileInfo.setFile(dir, "seek.ini");
            QFile::remove(inifileInfo.filePath());

            qInfo() << "计时结束,总共耗时:"<< m_downTime.elapsed() / 1000 << "s";
            emit sigDownFinish(fileInfo.filePath());
        }
        //流模式，计算所有的流数据然后发送
        else {
            QByteArray streamData;
            for(auto fileStreamData:m_listFtpData)
            {
                streamData += fileStreamData;
            }
            qInfo() << "计时结束,总共耗时:" << m_downTime.elapsed() / 1000 << "s";
            //发送流数据信号
            emit sigDownStreamFileFinish(streamData);
        }
    }
}

void SegmentedDownload::onDownloadDataUpdate(QByteArray downData, qint64 bytesReceived)
{
	qInfo() << "进来-----";
  //  QMutexLocker locker(&m_mutex);

//    //判断是哪一个指针发送的信号
//    if(FTPSocket * tempFtpSocket = static_cast<FTPSocket*>(sender()))
//    {
//        for(int i = 0; i < m_FTPSocketList.length(); i++)
//        {
//            if(m_FTPSocketList[i]== tempFtpSocket)
//            {
//                qInfo()<<"B---"<<QThread::currentThread();
//                //设置对应的进度直接和相关的ftp的下载数据；
//                m_ProgresslistByte[i] = bytesReceived;
//                m_listFtpData[i] = downData;

//                qInfo()<<"C---";
//                //进行最后的数据校验
//                if(m_ProgresslistByte[i] >= m_listDownSegmentedInterval[i].second && !m_downFinish[i])
//                {
//                    qInfo()<<"EEE---";

//                    m_downFinish[i] = true;
//                    m_listFtpData[i] =  m_listFtpData[i].mid(0, static_cast<int>(m_listDownSegmentedInterval[i].second - m_listDownSegmentedInterval[i].first));
//                }

//                qInfo()<<"D---";

//                //记录进度条的速度
//                qint64 progressBytes = 0;
//                for(auto progress : m_listFtpData)
//                {
//                    progressBytes += progress.length();
//                }
//                qInfo()<<"F---"<<progressBytes;
                //发送进度条信息
               // emit sigDownloadProgress(progressBytes, m_downFileSize);

//                //每1秒钟计算一次网速的bian变化

//                //记录每秒的实时下载网速
//                m_progressBytes = progressBytes;
//                m_nTime = m_downElapsedTimeCacle+m_downElapsedTime.elapsed();

//                //伪计时，类似于1秒钟写入文件一次
//            //    static qint64 count =0;

//                //如果是文件模式，则进行写文件操作
////                if(m_ftpModel == FTPMODE::MODE_FILE)
////                {
////                    count++;

////                    if(count%100 == 0)
////                    {
////                        if (m_fileResumeList.length() != 0)
////                        {
////                            //进行断线续传的文件数据写入
////                            m_fileResumeList[i]->open(QIODevice::WriteOnly);
////                            m_fileResumeList[i]->write(m_listFtpData[i]);
////                            m_fileResumeList[i]->flush();
////                            m_fileResumeList[i]->close();
////                        }
////                    }
////                }

//                break;
//            }
//        }
//    }
}

void SegmentedDownload::onDownloadTimeout()
{
    qInfo()<<"下载超时触发";
    emit sigaborFtpConnect();

    initAllListData();

    emit sigDownLoadTimeout();
}

void SegmentedDownload::onFtpClostConnect()
{
    qInfo() << "关闭连接"<< m_ftpCloseNum;
    m_isAllFinish = 3; //这边置为3就是为了让其不能进入
    m_ftpCloseNum++;

    //! 判断下载是否是活跃的，活跃的则关闭，然后重新开始计时，每来一个则重新计时一次，超时则触发信号
    if(m_downTimeoutTimer.isActive())
    {
        m_downTimeoutTimer.stop();
    }

    //m_downTimeoutTimer.start();
    if(m_ftpCloseNum >= m_maxThreadNum)
    {
        m_isAllFinish = 1;
        onDownloadDataFinish();
    }
}
