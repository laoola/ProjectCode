#include "FtpMultithreadDownLoad.h"
#include <QFileInfo>
#include <HelpClass.h>
#include <QThread>
#include <QEventLoop>
#include <QtMath>

FtpMultithreadDownLoad::FtpMultithreadDownLoad(QObject *parent):QObject(parent)
{

}

FtpMultithreadDownLoad::~FtpMultithreadDownLoad()
{
    for(auto & thread:m_thread)
    {
         if(thread->isRunning())
         {
             thread->quit();
             thread->wait();
         }

         delete thread;
         thread = nullptr;
    }

    m_thread.clear();

    for(auto & ftpClient:m_ftpClient)
    {
        delete ftpClient;
        ftpClient = nullptr;
    }

    m_ftpClient.clear();
}

FtpMultithreadDownLoad::FtpMultithreadDownLoad(FtpMultithreadDownLoad::FTPMODE ftpMode, int maxThreadNum):
    m_maxThreadNum(maxThreadNum),
    m_ftpRequestMode(ftpMode)
{

}

void FtpMultithreadDownLoad::setFtpRequestMode(FtpMultithreadDownLoad::FTPMODE ftpMode)
{
    m_ftpRequestMode = ftpMode;
}

void FtpMultithreadDownLoad::setDownMaxThreadNum(int threadNum)
{
    m_maxThreadNum = threadNum;
    m_oldMaxThreadNum = threadNum;
}

void FtpMultithreadDownLoad::fileSegmentation()
{
    m_progressSizeList.clear();

    //!进行文件分段。由程序进行分割一个整体文件
    if(m_isFileSegmented)
    {
        //根据线程数来平均分段下载的块数
        qint64 downPoint =  m_downFileSize/m_maxThreadNum;

        for(int i = 0; i < m_maxThreadNum; i++)
        {
            //初始化所有的进度值为0
            m_progressSizeList.append(0);

            //分段下载点的声明
            QPair<qint64, qint64> segmentedDownPoint;

            //赋值分段开始点
            segmentedDownPoint.first = static_cast<int>(i*downPoint);


            if(i+1 == m_maxThreadNum)
            {
                //!有时候分隔可能不是很均匀，所以，最后一个必须直接赋值文件本身的大小
                segmentedDownPoint.second = m_downFileSize;
            }
            else
            {
                segmentedDownPoint.second =static_cast<int>((i+1)*downPoint) ;
            }
            //每一个线程都有一个分段点
            m_segmentedDownloadPointList.append(segmentedDownPoint);


            //!下面开始进行断点续传的检查
            //!

            //赋值当前的下载的文件信息,进行文件的名称和路径的设置
            QFileInfo info(m_downFileName);

			QString fileName = info.fileName();
            QString suffix = info.suffix();

            QString tempDownFileName = fileName;
            tempDownFileName.remove(suffix);
            tempDownFileName+=PAR_FILE_SUFFIX;

            //获取保存文件的目录
            QDir dir = saveDownFileDir();
            QString tempParDownFile = tempDownFileName;
            QFileInfo fileInfo;
            tempParDownFile += QString::number(i);


            //赋值临时文件名称，后面下载会用到
            m_downTempFileNameList.append(tempParDownFile);


            fileInfo.setFile(dir, tempParDownFile);

            //如果文件存在说明需要断点续传，继续后面下载
            if(QFile::exists(fileInfo.filePath()))
            {
                QPair<QByteArray, int> tmepPair;

                //读取断点续传的文件
                QFile filehand (fileInfo.filePath());
                QByteArray data;
                if(!filehand.open(QIODevice::ReadOnly))
                {
                    qWarning()<<"file Open breadPoint read faile!!!";
                }
                else {
                    data = filehand.readAll();

                    filehand.close();
                }


                //对断点续传的数据和位置的保存赋值
                tmepPair.first = data;
                tmepPair.second = (i* downPoint) + data.length();

                m_breakpointdownList.append(tmepPair);
            }
            //!如果文件不存在，则直接赋值,不需要进行断点续传的记录
            else
            {
                QPair<QByteArray, int> tmepPair;
                tmepPair.first = "";
                tmepPair.second = m_segmentedDownloadPointList[i].first;

                m_breakpointdownList.append(tmepPair);

            }

        }
    }
    //!进入到这边，说明不需要由程序分割，下载的就是分割文件
    //! [小文件list下载]
    else
    {

        m_maxThreadNum = m_downFileNameList.length();

        //![分割断点设置]
        for(int i =0; i < m_maxThreadNum; i++)
        {
            //初始化所有的进度值为0
            m_progressSizeList.append(0);

            //分段下载点的声明
            QPair<qint64, qint64> segmentedDownPoint;
            //赋值分段开始点
            segmentedDownPoint.first = 0;   //永远从0开始下载，因为小文件时放在ftp上面分块好了的
            segmentedDownPoint.second = m_downFileSizeList[i];  //小文件的总大小

            //每一个线程都有一个分段点
            m_segmentedDownloadPointList.append(segmentedDownPoint);

            //!断点续传功能检查
            //!
            //赋值当前的下载的文件信息,进行文件的名称和路径的设置
            QFileInfo info(m_downFileNameList[i]);
            QString suffix = info.suffix(); //后缀名

            //解析需要下载保存的临时文件名称
            QString tempDownFileName = m_downFileNameList[i];
            tempDownFileName.remove(suffix);
            tempDownFileName+=PAR_FILE_SUFFIX;

            //!这里给下载文件复制后面需要合并的整体文件名称,因为分割原因，加个exe为了启动保险
            m_downFileName = tempDownFileName+".exe";

            //获取保存文件的目录
            QDir dir = saveDownFileDir();
            QString tempParDownFile = tempDownFileName;

            tempParDownFile += QString::number(i);

            //赋值临时文件名称，后面下载会用到
            m_downTempFileNameList.append(tempParDownFile);

            //这是目录和文件的关联
            QFileInfo fileInfo;
            fileInfo.setFile(dir, tempParDownFile);

            //!如果文件存在说明需要断点续传，继续后面下载
            if(QFile::exists(fileInfo.filePath()))
            {
                QPair<QByteArray, int> tmepPair;

                //读取断点续传的文件
                QFile filehand (fileInfo.filePath());
                QByteArray data;
                if(!filehand.open(QIODevice::ReadOnly))
                {
                    qWarning()<<"file Open breadPoint read faile!!!";
                }
                else
                {
                    data = filehand.readAll();

                    filehand.close();
                }


                //对断点续传的数据和位置的保存赋值
                tmepPair.first = data;
                tmepPair.second = data.length();

                m_breakpointdownList.append(tmepPair);
            }
            //!如果文件不存在，则直接赋值,不需要进行断点续传的记录
            //! [不进行断点续传设置]
            else
            {
                QPair<QByteArray, int> tmepPair;
                tmepPair.first = "";
                tmepPair.second = 0; //每次都从0开始下载，因为不是一整个文件分割的

                m_breakpointdownList.append(tmepPair);
            }
             //! [不进行断点续传设置]

        }
         //![分割断点设置]

    }
    //! [小文件list下载]
    //!
    //!
    //! 开始进行断点续传的比较
  

    for(int i = 0; i < m_breakpointdownList.length() ; i++)
    {
        //如果断点续传的开始点和分段文件的结束点一样，说明不需要断点续传，文件已经下好了，只需要等待合并即可
        if(m_breakpointdownList[i].second == m_segmentedDownloadPointList[i].second)
        {
            qInfo()<<"发现有已经下载好的文件，准备进行合并前的处理"<<i;
            //!移除这个记录点
            m_breakpointdownList.removeAt(i);
            m_segmentedDownloadPointList.removeAt(i);
            m_maxThreadNum --;
			i = -1;
        }
    }
}

int FtpMultithreadDownLoad::getDownFileSize(const QString &ftpHost, const QString &ftpUserName, const QString &ftpPasswd, const QString &cddirName, const QStringList & downFileName)
{

    QEventLoop eventLoop;
    FTPClient ftpSocket;

    //初始化连接失败为false，默认不会失败
    m_ftpConnectFaile = false;

    //连接到ftp服务器
    ftpSocket.connectFtpServer(ftpHost.toUtf8(), 21, ftpUserName.toUtf8(), ftpPasswd.toUtf8());

    //同步等待
    connect(&ftpSocket, &FTPSocket::sigFTPConnectFinish, &eventLoop, &QEventLoop::quit);
    
    //失败直接返回
    connect(&ftpSocket, &FTPClient::sigFTRConnectFaile, this, [=, &eventLoop]{
       m_ftpConnectFaile = true;
       eventLoop.quit();
    });

    eventLoop.exec();

    if(m_ftpConnectFaile)
    {
        emit this->sigFtpConnectFaile();
        return 0;
    }

    //如果切换目录的名称为空，则不进行目录切换，为了以防万一进行目录切换
    if(!cddirName.isEmpty())
    {
        ftpSocket.FtpCWD(cddirName);
    }

    //获取下载文件的大小
    int fileSize  = 0;

    for(int i = 0; i < downFileName.length(); i++)
    {
        m_downFileSizeList.append(static_cast<int>(ftpSocket.FtpFileSize(downFileName.at(i))));
        //计算总的文件大小
        fileSize += m_downFileSizeList[i];
    }


    //关闭ftp的链接
    ftpSocket.closeFTPConnect();

    return fileSize;
}

QDir FtpMultithreadDownLoad::saveDownFileDir()
{

    QString dirPath = HelpClass::getCurrentAppDataDir();
    dirPath = dirPath+"/downFile";

    bool ok = HelpClass::isDirExist(dirPath);
    qInfo()<<"创建目录成功 "<<ok;
    qInfo()<<"save file path dir = "<<dirPath;
    QDir dir(dirPath);
    if(!dir.exists())
    {
       bool ok =  dir.mkdir(dirPath);
       if(!ok)
       {
           qWarning()<<dirPath<<" mkdir failte!!";
       }
    }


    return dir;
}

void FtpMultithreadDownLoad::mergeAllParFile()
{

    qInfo()<<"下载完成，启动线程: "<<m_maxThreadNum<<"  一共花费时间是: "<<m_downTestTotalTime.elapsed()/1000.0;

    //停止定时器的更新
    m_downTimer.stop();

    QDir saveDir = saveDownFileDir();

	QFileInfo fileNameInfo(m_downFileName);


    QFileInfo fileInfo(saveDir, fileNameInfo.fileName());

    QFile fileHand(fileInfo.filePath());
    if(!fileHand.open(QIODevice::WriteOnly))
    {
        qWarning()<<"merge Par file Faile, open Failte!!!";

        return;
    }

    qInfo()<<"m_downTempFileNameList "<<m_downTempFileNameList;
    for(int i =0; i < m_maxThreadNum; i++)
    {
        QString downTempfileName = m_downTempFileNameList[i];
        QFileInfo fileInfoTempFile(saveDir, downTempfileName);
        QFile file(fileInfoTempFile.filePath());

        if(!file.open(QIODevice::ReadOnly))
        {
            qWarning()<<"open downTempfileName failte!!";
            return;
        }

        fileHand.write(file.readAll());
        file.close();

        //文件移除
        file.remove();
    }

	fileHand.close();

    //关闭停止线程，并删除
    for(auto thread:m_thread)
    {
        thread->quit();
        thread->wait();

        delete  thread;
        thread = nullptr;
    }

    m_thread.clear();

    //先全部退出一遍，然后清除
    for(auto ftpclient:m_ftpClient)
    {
          ftpclient->closeFTPConnect();

          delete  ftpclient;
          ftpclient =nullptr;
    }

    m_ftpClient.clear();

    if(m_ftpRequestMode == FTPMODE::MODE_FILE)
    {
       
        //发送保存完毕的信号
        emit sigDownFinish(fileInfo.filePath());
    }
    else
    {
		if (!fileHand.open(QIODevice::ReadOnly))
		{

			qWarning() << "merge Par file Faile, open Failte!!!";

			return;
		}
        QByteArray dataStream = fileHand.readAll();

		fileHand.close();

        fileHand.remove();

		qInfo() << "dataStream" << dataStream.length();
        //发送流信号
        emit sigDownStreamFinish(dataStream);
    }

}

void FtpMultithreadDownLoad::initdownTimerSpeedTimer()
{
   //测试用，看下载总共需要话费所少时间
   m_downTestTotalTime.start();

   connect(&m_downTimer, &QTimer::timeout, this, &FtpMultithreadDownLoad::onDownSpeedTime);

    //开始计算下载时间（计算下载速度时用到的时间，并不是定时器）
    m_downloadTime.start();

    //!启动计算下载速度计算的定时器，每隔一秒计算更新一下
    m_downTimer.setInterval(1000);
    m_downTimer.start();
}

void FtpMultithreadDownLoad::ftpStartDown(const QString &ftpHost, const QString &ftpUserName, const QString &ftpPasswd,  const QString & chdirName, const QString &downFileName)
{
    //需要进行下载文件的自动切割
    m_isFileSegmented = true;


    m_downFileName = downFileName;

    //单文件只有一个
    QStringList getFileNameList(downFileName);

    //获取文件大小
    m_downFileSize = getDownFileSize(ftpHost, ftpUserName, ftpPasswd, chdirName, getFileNameList);

	if (m_downFileSize == 0)
	{
        emit sigFtpFileError(tr("Request file does not exist"));
		return;
	}
    //初始化所有的下载是没有完成，默认是0
    m_downFinishNum = 0;

    //进行文件切割
    fileSegmentation();

    //qInfo()<<"currrent Thread ="<<QThread::currentThread();

    qInfo()<<"m_segmentedDownloadPointList"<<m_segmentedDownloadPointList;
    qInfo()<<"m_breakpointdownList"<<m_breakpointdownList;

	//如果等于0说明全是下载好的文件，只需要合并即可
	if (m_breakpointdownList.length() == 0 && m_segmentedDownloadPointList.length() == 0)
	{
        m_maxThreadNum = m_oldMaxThreadNum;
		mergeAllParFile();

		return;
	}

    //!启动正式下载操作
    for(int i = 0; i < m_maxThreadNum; i++)
    {
        FTPClient * tempFtpClient = new FTPClient(m_breakpointdownList[i], m_segmentedDownloadPointList[i],m_downTempFileNameList.at(i));
        QThread * tempThread = new QThread;

        m_ftpClient.append(tempFtpClient);
        m_thread.append(tempThread);

        connect(tempFtpClient, &FTPClient::sigDownProgressBytes, this, &FtpMultithreadDownLoad::onFtpDownFileUpdateState);
        connect(this, &FtpMultithreadDownLoad::sigConnectFtpClient, tempFtpClient, &FTPClient::onConnectFtpClient);
        //下载中断后触发
        connect(tempFtpClient, &FTPClient::sigFtpConnectInterruption, this, &FtpMultithreadDownLoad::sigFtpDownNoResponse);

        tempFtpClient->moveToThread(tempThread);

        tempThread->start();

        emit sigConnectFtpClient(ftpHost, ftpUserName, ftpPasswd,chdirName, downFileName,  tempThread);
    }


    initdownTimerSpeedTimer();

}

void FtpMultithreadDownLoad::ftpStartDown(const QString &ftpHost, const QString &ftpUserName, const QString &ftpPasswd, const QString & chdirName,  const QStringList &downFileNameList)
{
    m_isFileSegmented = false;


    //保存好下载的零时文件文件名称
    m_downFileNameList = downFileNameList;

    //获取文件大小
    m_downFileSize = getDownFileSize(ftpHost, ftpUserName, ftpPasswd, chdirName, downFileNameList);

    //文件为空时触发文件错误
	if (m_downFileSize == 0)
	{
        emit sigFtpFileError(tr("Request file does not exist"));
        return;
	}

    //初始化所有的下载是没有完成，默认是0
    m_downFinishNum = 0;

    //进行文件切割
    fileSegmentation();

	//如果等于0说明全是下载好的文件，只需要合并即可
	if (m_breakpointdownList.length() == 0 && m_segmentedDownloadPointList.length() == 0)
	{
        m_maxThreadNum = m_oldMaxThreadNum;
		mergeAllParFile();

		return;
	}

    //qInfo()<<"currrent Thread ="<<QThread::currentThread();

    //qInfo()<<"m_segmentedDownloadPointList = "<<m_segmentedDownloadPointList;

    //启动正式下载操作
    for(int i = 0; i < m_maxThreadNum; i++)
    {
        FTPClient * tempFtpClient = new FTPClient(m_breakpointdownList[i], m_segmentedDownloadPointList[i],m_downTempFileNameList.at(i));
        QThread * tempThread = new QThread;

        m_ftpClient.append(tempFtpClient);
        m_thread.append(tempThread);

        tempFtpClient->moveToThread(tempThread);

        connect(tempFtpClient, &FTPClient::sigDownProgressBytes, this, &FtpMultithreadDownLoad::onFtpDownFileUpdateState);
        connect(this, &FtpMultithreadDownLoad::sigConnectFtpClient, tempFtpClient, &FTPClient::onConnectFtpClient);

        connect(tempFtpClient, &FTPClient::sigFTRConnectFaile, this, &FtpMultithreadDownLoad::sigFtpConnectFaile);

        tempThread->start();

        emit sigConnectFtpClient(ftpHost, ftpUserName, ftpPasswd,chdirName, m_downFileNameList[i],  tempThread);
    }

    initdownTimerSpeedTimer();
}

void FtpMultithreadDownLoad::onFtpDownFileUpdateState(int downSize, bool isFinish)
{
    qInfo()<<"下载信号通知进来";
    qInfo()<<"downSize"<<downSize;
    qInfo()<<"isFinish"<<isFinish;

    for(int i = 0;i < m_ftpClient.length(); i++)
    {
         if(m_ftpClient[i] == static_cast<FTPClient*>(sender()))
         {

			 int progressSize = 0;
             //! 传送过来的就是下好的实际大小
             m_progressSizeList[i] = downSize;

             //更新当前的进度
             for(auto progree:m_progressSizeList)
             {
                 progressSize +=progree;
             }

             m_bytesReceivedSize = progressSize;

             break;
         }
    }
    //如果为真则记录进来的完成的数量，如果正好等于线程的总数量，说明全部下载完成，可以进行合并操作
    if(isFinish)
    {
        m_downFinishNum++;

        qInfo()<<"m_downFinishNum"<<m_downFinishNum;
        //全部完成，进行合并
        if(m_downFinishNum == m_maxThreadNum)
        {
            //经行文件合并
            mergeAllParFile();
        }
    }

}

void FtpMultithreadDownLoad::onDownSpeedTime()
{
    //!更新到ui的数据
    emit sigDownProgressUpdate(m_bytesReceivedSize, m_downFileSize);

    //计算下载的时间
   int nTime  = m_downloadTime.elapsed();
   nTime  -= m_nTime;

  // 下载速度
   double dBytesSpeed = (m_bytesReceivedSize * 1000.0) / nTime ;
   double dSpeed = dBytesSpeed;

   //剩余时间
   qint64 leftBytes = (m_downFileSize - m_bytesReceivedSize);
   double dLeftTime = (leftBytes * 1.0) / dBytesSpeed;


   // 获取上一次的时间
    m_nTime = nTime;

    //更行当前的下载速度和剩余时间
    sigDownSpeedTimeUpdate( HelpClass::speed(dSpeed), HelpClass::timeFormat(qCeil(dLeftTime)));
}



FTPClient::FTPClient(QObject *parent): FTPSocket(parent)
{
    m_deleteSelf = false;
    m_downFileDir = saveDownFileDir();

    QFileInfo fileInfo;

    fileInfo.setFile(m_downFileDir, "Temp");

	QString filePath = fileInfo.filePath();  //包括文件名的文件路径
    m_fileHand.setFileName(filePath);
}

FTPClient::FTPClient(QPair<QByteArray, int> breakpointdown,  QPair<int, int> segmentedDownloadPoint, QString parfileName)
{
    m_deleteSelf = false;
    m_downFileDir = saveDownFileDir();

    m_breakpointdown = breakpointdown;
    m_segmentedDownloadPoint = segmentedDownloadPoint;
    QFileInfo fileInfo;

    fileInfo.setFile(m_downFileDir, parfileName);

	QString filePath = fileInfo.filePath();  //包括文件名的文件路径
    m_fileHand.setFileName(filePath);
}

FTPClient::~FTPClient()
{
    m_deleteSelf = true;
    this->aborFtpAllConnect();
}


void FTPClient::onFtpDataRead()
{
    if(m_deleteSelf)
    {
        return;
    }

    QByteArray fileData = m_FtpDataSocket->readAll();

    m_ftpCacheData += fileData;

    int fileDataSize = m_segmentedDownloadPoint.first + m_ftpCacheData.length(); //起点下载位置加上一共下载的大小属于总共结束大小

    qInfo()<<"fileDataSize"<<fileDataSize;
   qInfo()<<"m_segmentedDownloadPoint.second"<<m_segmentedDownloadPoint.second;

    //!进行判断处理，大于下载点必须停止
    if (fileDataSize >= m_segmentedDownloadPoint.second )
    {
        aborFtpDataConnect();
        closeFTPConnect();
        QByteArray tempData  = m_ftpCacheData.mid(0, m_segmentedDownloadPoint.second - m_segmentedDownloadPoint.first);

        //qInfo()<<"tempData Length = "<<tempData.length();
        //qInfo() << "长度 = ." << m_segmentedDownloadPoint.second - m_segmentedDownloadPoint.first;
        qInfo()<<"m_fileHand ="<<m_fileHand.fileName();
        if(!m_fileHand.isOpen())
        {
            if(!m_fileHand.open(QIODevice::WriteOnly))
            {
                qWarning()<<"ftp Write File Data Failte!";
            }
            else
            {
                m_fileHand.write(tempData);
                m_fileHand.flush();
                m_fileHand.close();
            }
        }

        //qInfo()<<"下载完成，准备发射型号岛外";
        //下载完成， true
        emit sigDownProgressBytes(tempData.length(), true);

        //清空
        m_ftpCacheData.clear();
    }
    else
    {
        if(!m_fileHand.isOpen())
        {
            if(!m_fileHand.open(QIODevice::WriteOnly))
            {
                qWarning()<<"ftp Write File Data Failte!";

            }
            else
            {
                m_fileHand.write(m_ftpCacheData);
                m_fileHand.flush();
                m_fileHand.close();
            }
        }
        qInfo()<<"写入数据完成，准备更新数据到外面";

        //更新下载状态，和进度
        emit sigDownProgressBytes(m_ftpCacheData.length(), false);
    }


}

QDir FTPClient::saveDownFileDir()
{
    QString dirPath = HelpClass::getCurrentAppDataDir();
    dirPath = dirPath+"/downFile";

    QDir dir(dirPath);
    if(!dir.exists())
    {
        dir.mkdir(dirPath);
    }

    return dir;
}

//  const QPair<QByteArray, int> & breakpointdown, const QPair<int, int> & segmentedDownloadPoint,
void FTPClient::onConnectFtpClient(const QString &ftpHost, const QString &ftpUserName, const QString &ftpPasswd, const QString &chdirName, const QString &downFileName, QThread *threadID)
{
    //qInfo()<<"current thread"<< QThread::currentThread();
    //qInfo()<<"threadID"<< threadID;
    //如果线程id与当前的线程id部队，直接返回return
    if(threadID != nullptr && threadID != QThread::currentThread())
    {
        return;
    }
    qInfo()<<"信号下载触发-=============";

    //连接前的准备工作
    // m_segmentedDownloadPoint = segmentedDownloadPoint;
    m_cdDirName = chdirName;
    // m_breakpointdown = breakpointdown;
    m_downFtpFileName = downFileName;

    //启动连接ftp
    this->connectFtpServer(ftpHost.toUtf8(), 21, ftpUserName.toUtf8(), ftpPasswd.toUtf8());


    //连接ftp成功后启动下载文件
    connect(this, &FTPClient::sigFTPConnectFinish, this, &FTPClient::onFtpDownFile);

    //connect(this, &FTPClient::sigFtpConnectInterruption, this, &FTPClient::onFtpReconnect);
}

void FTPClient::onFtpDownFile()
{
    //不是空就进来切换ftp的目录
    if(!m_cdDirName.isEmpty())
    {
        //切换目录
        FtpCWD(m_cdDirName);
    }

    //设置数据的下载为二进制
    writeCommand(QByteArray("TYPE I\r\n"));

    m_ftpCacheData = "";

    //断点续传进行预备设置准备
    if(m_breakpointdown.second != 0)
    {
        m_ftpCacheData = m_breakpointdown.first;
        writeCommand(QString("REST %1\r\n").arg(m_breakpointdown.second));
    }

    //切换成被动模式
    pasvMode();

    writeCommand(QString("RETR %1\r\n").arg(m_downFtpFileName));
}

//void FTPClient::onFtpReconnect()
//{
//    this->closeFTPConnect();


//}
