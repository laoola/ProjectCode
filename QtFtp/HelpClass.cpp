#include "HelpClass.h"
#include <QDateTime>
#include <QCryptographicHash>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QUuid>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QSettings>
#include <QVariant>
#include <QApplication>
#include <QFontMetrics>
#include <QXmlStreamAttributes>
#include <QDirIterator>

#ifndef QMLAPP
#include "ToolTip.h"
#include "FGraphicsEffect.h"
#endif

#include <QClipboard>

#include <QProcess>
#include <QXmlStreamReader>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
///翻译用到的头文件
#include <QTranslator>
#include <QLocale>
///!

#include "qaesencryption.h"
#include <QQmlApplicationEngine>

QJsonObject HelpClass::m_jsonObject;
QLabel* HelpClass::m_label = nullptr;
QTranslator HelpClass::m_translator;

HelpClass::HelpClass(QObject *parent) : QObject(parent)
{

}

//template<typename T1,typename T2,typename T3>
//T2 HelpClass::dynameLoaderDllFile(const T1 & dllFile, const T2 & t2,  const T3 & funcName)
//{
//#ifdef Q_OS_WIN32

//    const char * dllStream = dllFile.data();
//    HMEMORYMODULE hm = MemoryLoadLibrary(dllStream);

//    QByteArray funcNameByte = funcName.toUtf8();
//    const char * funNameStr = funcNameByte.data();

//    t2 = (t2)(MemoryGetProcAddress(hm, funNameStr));

//    return  t2;
// //   return  nullptr;
//#endif
//}

void HelpClass::onlyOpenOnceEXE()
{
    //限制重复打开
    QSystemSemaphore * semaphore = new QSystemSemaphore("ProgramKey",1, QSystemSemaphore::Open);
    semaphore->acquire();
    //在临界区操作共享内存SharedMemory
    QSharedMemory * memory = new QSharedMemory("Program");//全局对象名
    if(!memory->create(1)) //如果全局对象存在则提示退出
    {
        QMessageBox::information(nullptr, QObject::tr("Tip"), QObject::tr("Program hasbeen running!"));
        semaphore->release();

		delete semaphore;
		semaphore = nullptr;

		delete memory;
		memory = nullptr;
        qApp->quit();
        //必须要exit，不能使用qApp->exit(0)或者quit();qt的方法退步出去
        exit(0);
    }
    semaphore->release();
}

QByteArray HelpClass::aes128Encryption(const QByteArray &key, const QByteArray &plaintext)
{
    if (plaintext.isEmpty())
    {
        return QByteArray("");
    }

    if (key.length() != 16)
    {
        qWarning() << "encryption key size is" << key.length();
        return QByteArray();
    }

    //解密
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB , QAESEncryption::ZERO);

    QByteArray encodedText = encryption.encode(plaintext, key);


    return  encodedText;
}

QByteArray HelpClass::aes128Decrypt(const QByteArray &key, const QByteArray &encodedText)
{
    if (encodedText.isEmpty())
    {
        return "";
    }

    //解密
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB , QAESEncryption::ZERO);

    QByteArray decodedText = encryption.decode(encodedText, key);

    //qInfo() << "decodedText" << decodedText;

    QByteArray decodedString = (QAESEncryption::RemovePadding(decodedText,QAESEncryption::ZERO));
    //qDebug() << "decodedString"<< decodedString;

    return decodedString;
}

QList<int> HelpClass::random(int nMax, int nCount, bool trueRandom)
{
    QList<int> intList;
    int   i=0, m=0;
    QTime time;
    for(i=0;;)
    {
        if (intList.count() > nCount)
            break;

        int     randn;
        if(trueRandom)
        {
            time    = QTime::currentTime();
            qsrand(time.msec()*qrand()*qrand()*qrand()*qrand()*qrand()*qrand());
        }

        randn   = qrand()%nMax;
        m=0;

        while(m<i && intList.at(m)!=randn)
            m++;

        if(m==i)            { intList.append(randn); i++;}
        else if(i==nMax)    break;
        else                continue;
    }

    return intList;
}

void HelpClass::replaceCharacters(QString &chara)
{
    if(chara.indexOf("\\n") != -1)
    {
        chara = chara.replace("\\n", "\n");
    }
    if(chara.indexOf("\\r") != -1)
    {
        chara = chara.replace("\\r", "\r");
    }
    if(chara.indexOf("\\t") != -1)
    {
        chara = chara.replace("\\t", "\t");
    }
}
int HelpClass::languageSettings(const QStringList &languageQmFile , bool autoLoader, int loaderType)
{
    //开机自动识别软件语言
    int language = -1;  //0代表英文环境,1代表中文，-1代表没有检测到系统语言
    //#define ENGLISH
    //如果是英语,测试伪英文环境
#ifdef ENGLISH
    translator.load(QString(":/qm/os_language_English.qm"));  //选择翻译文件
    qApp->installTranslator(&translator);
#else
    //自动根据环境来加载
    if(autoLoader)
    {
        QLocale locale;
        QString languageFile = "";
        QLocale::Language currentSystemLanguage =  locale.language() ;
        qInfo()<<"当钱的系统环境是"<<currentSystemLanguage;
        if(currentSystemLanguage == QLocale::English )  //获取系统语言环境
        {
            language = 0;
            qDebug() << "English system" ;
            languageFile = languageQmFile.at(language);
        }
        else if( currentSystemLanguage == QLocale::Chinese )
        {

            qDebug() << "中文系统,下面要开始区分简体和繁体区别";
            //查询当前国别代码
            //增加多语言支持
            language = 1;
            if(locale.country() == QLocale::China)
            {
                //中文简体
                language = 1;
                languageFile = languageQmFile.at(language);
            }
            else if(locale.country() == QLocale::Taiwan || locale.country() == QLocale::HongKong)
            {
                //中文繁体
                language = 2;
                languageFile = languageQmFile.at(language);
            }

        }
        else if(currentSystemLanguage == QLocale::Japanese)
        {
            //中文简体
            language = 3;
            languageFile = languageQmFile.at(language);
        }
        else{
            language = 0;
            qDebug() << "default English system" ;
            languageFile = languageQmFile.at(language);
        }

        //翻译文件非空则加载翻译文件
        if(!languageFile.isEmpty())
        {
            m_translator.load(languageFile);
            qApp->installTranslator(&m_translator);
        }

    }
    else
    {
        //先移除
        qApp->removeTranslator(&m_translator);

        //手动加载翻译,直接根据给定的loaderType来加载即可
        bool ok = m_translator.load(languageQmFile.at(loaderType)); //选择翻译文件
        language = loaderType;
        qInfo()<<"ok"<<ok;
        qApp->installTranslator(&m_translator);

    }

#endif

    return language;
}

QByteArray HelpClass::toHexBase64(QByteArray data)
{
    QByteArray hexData = data.fromHex(data);

    return hexData.toBase64();
}

QByteArray HelpClass::toHexBase64(QString data)
{
    return toHexBase64(data.toUtf8());
}

void HelpClass::modifyFileTime(const QString &filePathName, const QByteArray &date, const QByteArray &time)
{
#ifdef Q_OS_WIN32
    SYSTEMTIME spec_time;

    sscanf(date.data(),"%d-%d-%d", &spec_time.wYear, &spec_time.wMonth, &spec_time.wDay);
    sscanf(time.data(),"%d:%d:%d", &spec_time.wHour, &spec_time.wMinute, &spec_time.wSecond);
    spec_time.wDayOfWeek    = 1;
    spec_time.wMilliseconds = 0;
    // TODO: 此处省略对时间有效性的校验

    //   char * FILEC = (char *)filePathName.data();
    //   WCHAR wFILEC[128] = {0};
    //   mbstowcs(wFILEC,FILEC, strlen(FILEC));
    // 获取文件句柄
    HANDLE hFile = CreateFile(filePathName.toStdWString().c_str(),      // LPCTSTR lpFileName,
                              GENERIC_READ|GENERIC_WRITE,         // DWORD dwDesiredAccess,
                              FILE_SHARE_READ|FILE_SHARE_DELETE,
                              NULL,                               // LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                              OPEN_EXISTING,
                              FILE_FLAG_BACKUP_SEMANTICS,         // DWORD dwFlagsAndAttributes,
                              NULL);

    if( hFile == INVALID_HANDLE_VALUE ) {
        qWarning("Get file handle failed, error = %s\n", GetLastError());
        return;
    }

    // 设置文件时间
    FILETIME ft,LocalFileTime;
    SystemTimeToFileTime(&spec_time, &ft);
    LocalFileTimeToFileTime(&ft,&LocalFileTime);

    if(SetFileTime(hFile,&LocalFileTime,(LPFILETIME) NULL,&LocalFileTime)) {
        printf("Set file time success\n");
    }else{
        printf("Set file time failed\n");
    }
    CloseHandle(hFile);
#endif
}

QByteArray HelpClass::GetEncrypt(const QByteArray plaintextStr, QByteArray key)
{
    QByteArray encryptText = encryption(plaintextStr);
    if(key.isEmpty())
    {
        key = DefaultKEY;
    }
    return getXorEncryptDecrypt(encryptText, key);
}

QByteArray HelpClass::GetEncrypt(const QString plaintextStr, QByteArray key)
{
    QByteArray encryptText = encryption(plaintextStr);
    if(key.isEmpty())
    {
        key = DefaultKEY;
    }
    return getXorEncryptDecrypt(encryptText, key);
}

QByteArray HelpClass::GetDecrypt(const QByteArray ciphertext, QByteArray key)
{
    if(key.isEmpty())
    {
        key = DefaultKEY;
    }

    QByteArray plaintextByte = getXorEncryptDecrypt(ciphertext, key);

    return Deciphering(plaintextByte);
}

void HelpClass::hideDirFile(QString dirPath)
{
#ifdef Q_OS_WIN32
    SetFileAttributes(reinterpret_cast<LPCWSTR>(dirPath.unicode()),FILE_ATTRIBUTE_HIDDEN);
#endif

#ifdef Q_OS_LINUX
    QFile file(dirPath);
    QFileInfo fileInfo(file);
    QString path = fileInfo.absolutePath();
    QString fileName = fileInfo.fileName();
    qInfo()<<"path = "<<path;
    qInfo()<<"fileName = "<<fileName;
    QString newFileName = path+"."+fileName;
    file.rename(newFileName);
#endif
}

void HelpClass::showDirFile(QString dirPath)
{
#ifdef Q_OS_WIN32
    SetFileAttributes(reinterpret_cast<LPCWSTR>(dirPath.unicode()),FILE_ATTRIBUTE_DIRECTORY);
#endif
}

bool HelpClass::isDirExist(QString fullPath)
{
  

    QDir dir(fullPath);


    if(dir.exists())
    {
        //qDebug() << "dir exists" << fullPath;
        return true;
    }
    else
    {
        qDebug() << "create path fullPath";
        bool ok = dir.mkpath(fullPath);//创建多级目录
		
        return ok;
    }
}

bool HelpClass::isStrContainsNumbers(const QString &str)
{
    std::string temp = str.toStdString();

    for (int i = 0; i < str.length(); i++)
    {
        if (temp[i] >='0' && temp[i]<= '9' )
        {
            return true;
        }
    }

    return false;
}

void HelpClass::fileDataWrite(const QString &filePath, const QByteArray &fileData)
{
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(fileData);

        file.close();
    }
    else
    {
        qWarning()<<"文件路径"+filePath+"打开失败";
    }


}

void HelpClass::checkFileSuffix(QString &filePath, const QString &suffix)
{
    //检查不区分大小写
    if(!filePath.endsWith(suffix, Qt::CaseInsensitive))
    {
        int index = suffix.indexOf(".");
        QString tempSuffix = suffix;
        //如果不含有后缀点，则自动补上点
        if(index == -1)
        {
            tempSuffix = "."+ tempSuffix;
        }

        filePath += tempSuffix;
    }
}

QString HelpClass::bytesToKB(qint64 size)
{
    qint64 rest = 0.00;
    if(size < 1024)
    {
        return QString::number(size, 'f', 2) + "B";
    }
    else
    {
        size /= 1024;
    }
    if(size < 1024)
    {
        return QString::number(size, 'f', 2) + "KB";
    }
    else
    {
        rest = size % 1024;
        size /= 1024;
    }
    if(size < 1024)
    {
        size = size * 100;
        return QString::number((size / 100), 'f', 2) + "." + QString::number((rest * 100 / 1024 % 100), 'f', 2) + "MB";
    }
    else
    {
        size = size * 100 / 1024;
        return QString::number((size / 100), 'f', 2) + "." + QString::number((size % 100), 'f', 2) + "GB";
    }
}

bool HelpClass::removeFile(QString filePath)
{
    if(filePath.isEmpty())
    {
        // filePath = getCurrentTempDataDir()+"/"+AgencyFileName;
        qWarning("delete File is Empty!!!");
        return false;
    }
    QFile * file = new QFile(filePath);
    if(file->exists())
    {
        //删除成功
        if(file->remove())
        {
            delete file;
            return true;
        }
        else
        {
            //删除失败,强制删除
            forcedDeleteFile(filePath);

            delete file;
            return false;
        }
    }
    else
    {
        qCritical("File '%s' does not exist!", qUtf8Printable(filePath));
    }
    delete file;
    return false;
}

bool HelpClass::removeDirFile(const QString &dirPath)
{
    QString tmepFilePath = "";

    if(!dirPath.isEmpty())
    {
        tmepFilePath = dirPath;
    }
    else
    {
        tmepFilePath = getCurrentTempDataDir() +"/.QQMasters";
    }

//    qDebug()<<"tmepFilePath = "<<tmepFilePath;
//    QDir dir(tmepFilePath);
//    dir.setFilter(QDir::Files);
//    int  fileCount =static_cast<int>(dir.count());
//    qDebug()<<"fileCount = "<<fileCount;
//    bool removeOk = true;
//    for (int  i = 0; i < fileCount; i++)
//    {
//        qDebug()<<"dir[i]) = "<<dir[i];
//        //如果一个删除失败，则返回false
//        if(!dir.remove(dir[i]))
//        {
//            removeOk = false;
//        }
//    }

    qInfo()<<"tmepFilePath"<<tmepFilePath;
    QDir Dir(tmepFilePath);
    bool removeOk = true;
    if(Dir.isEmpty())
    {
        qInfo()<<"dir is Empty";
        return !removeOk;
    }

    // 第三个参数是QDir的过滤参数，这三个表示收集所有文件和目录，且不包含"."和".."目录。
    // 因为只需要遍历第一层即可，所以第四个参数填QDirIterator::NoIteratorFlags
    QDirIterator DirsIterator(tmepFilePath, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while(DirsIterator.hasNext())
    {
        if (!Dir.remove(DirsIterator.next())) // 删除文件操作如果返回否，那它就是目录
        {
            QDir(DirsIterator.filePath()).removeRecursively(); // 删除目录本身以及它下属所有的文件及目录
        }
    }

    return removeOk;

}

void HelpClass::forcedDeleteFile(QString filePath)
{
    qWarning("启动强制删除文件操作!!!");
#ifdef Q_OS_WIN32
    QProcess process;
    qInfo()<<"filePath<<"<<filePath;

    filePath =  filePath.replace("/", "\\");

    process.start(QString("del /f /s /q %1").arg(filePath));
    process.waitForStarted();
    process.waitForFinished();
#endif
#ifdef Q_OS_LINUX
    QProcess::execute(QString("rm -rf  %0").arg(QString(filePath)));
#endif
}

QString HelpClass::getTimestamp()
{
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = static_cast<int>(time.toTime_t()); //获取时间戳

    return QString::number(timeT);
}

QByteArray HelpClass::toHexMd5Encrypt(const QByteArray & plaintextStr)
{
    if(plaintextStr.isEmpty())
    {
        return "";
    }
    //QByteArray plaintextByArray = plaintextStr.toUtf8();
    QByteArray byteMd5 = QCryptographicHash::hash(plaintextStr, QCryptographicHash::Md5);
    QByteArray strPwdMd5 = byteMd5.toHex();

    return strPwdMd5;
}

QByteArray HelpClass::md5Encrypt(QString plaintextStr)
{
    if(plaintextStr.isEmpty())
    {
        return "";
    }
    QByteArray plaintextByArray = plaintextStr.toUtf8();
    QByteArray byteMd5 = QCryptographicHash::hash(plaintextByArray, QCryptographicHash::Md5);
    // QByteArray strPwdMd5 = byteMd5.fromHex(byteMd5);

    return byteMd5;
}

QString HelpClass::getCurrentTime(const QString &format)
{
    QTime  currenttime = QTime::currentTime();
    if(format.isEmpty())
    {
        return currenttime.toString();
    }

    return currenttime.toString(format);
}

QString HelpClass::getCurrentDateTime(const QString &format)
{
    QDateTime date =  QDateTime::currentDateTime();

    if(format.isEmpty())
    {
        return date.toString();
    }

    return date.toString(format);
}

QString HelpClass::msecsTo(const QString &timeStr)
{
    QTime time = QTime::fromString(timeStr);
    int mesecs = time.msecsTo(QTime::currentTime());

    return QString::number(mesecs);
}

void HelpClass::jsonCreate(const QString &key, const QJsonValue &value, bool isClean)
{
    if(isClean)
    {
        QStringList keyList = m_jsonObject.keys();
        for(QString key : keyList)
        {
            m_jsonObject.remove(key);
        }
    }
    if(key.isEmpty())
    {
        return;
    }
    m_jsonObject.insert(key, value);
}

QJsonObject HelpClass::getJsonObject()
{
    return m_jsonObject;
}

QString HelpClass::tojsonArray(QJsonArray jsonArray)
{
    QJsonDocument document;
    document.setArray(jsonArray);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    return strJson;
}

bool HelpClass::generalJsonParse(QJsonDocument jsonDocument, QMap<QString, QString> &jsonkeyMap, QString headKey, int headValue, bool flage)
{
    if(jsonDocument.isObject())
    {
        QJsonObject object = jsonDocument.object();

        if(object.contains(headKey) || flage)
        {
            int code = -1;
            //如果为false才截取
            if(!flage)
            {
                code  = object.value(headKey).toInt();
            }

            if(flage || code== headValue)
            {
                QStringList keyList = jsonkeyMap.keys();

                for(QString key : keyList)
                {
                    if(object.contains(key))
                    {
                        QJsonValue value = object.value(key);
                        QVariant varVariant = value.toVariant();

                        jsonkeyMap[key] = varVariant.toString();
                    }
                }
                return true;
            }
            else
            {
                QStringList keyList = jsonkeyMap.keys();
                for(QString key : keyList)
                {
                    if(object.contains(key))
                    {
                        QJsonValue value = object.value(key);
                        QVariant varVariant = value.toVariant();
                        jsonkeyMap[key] = varVariant.toString();
                    }
                }
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    if(jsonDocument.isArray())
    {
        QJsonArray array = jsonDocument.array();
        for(int i = 0; i < array.size(); i++)
        {
            QJsonValue value =  array.at(i);

            if(value.isObject())
            {
                QJsonObject object = array.at(i).toObject();
                if(flage || object.contains(headKey))
                {
                    if(flage  || object.take(headKey)== headValue)
                    {
                        QStringList keyList = jsonkeyMap.keys();

                        for(QString key : keyList)
                        {
                            if(object.contains(key))
                            {
                                QJsonValue value = object.value(key);
                                QVariant varVariant = value.toVariant();
                                jsonkeyMap[key] = varVariant.toString();
                            }
                        }
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }

        }
    }

    return false;
}

bool HelpClass::generalArrayJsonParse(QJsonDocument jsonDocument, QList<QMapString> &listJsonkepMap, QMapString jsonkey, QString arrayKey, QString headKey, int headValue,bool flage)
{
    //只判断是对象的情况，数组情况目前不做判断
    if(jsonDocument.isObject())
    {
        QJsonObject jsonObject = jsonDocument.object();

        if(jsonObject.contains(headKey) || flage)
        {
            if(jsonObject.take(headKey).toInt() == headValue || flage)
            {
                QJsonValue jsonValue = jsonObject.value(arrayKey);

                //进行判断，是否是数组
                if(jsonValue.isArray())
                {
                    QJsonArray array = jsonValue.toArray();

                    for(int i = 0; i < array.size(); i++)
                    {
                        QJsonValue value =  array.at(i);

                        if(value.isObject())
                        {
                            QJsonObject object = value.toObject();

                            QStringList keyList = jsonkey.keys();
                            QMapString tempMap;
                            for(QString key : keyList)
                            {
                                if(object.contains(key))
                                {
                                    QJsonValue value = object.value(key);
                                    QVariant varVariant = value.toVariant();

                                    tempMap[key] = varVariant.toString();

                                }
                            }
                            listJsonkepMap.append(tempMap);
                        }
                        else if(value.isString())
                        {
                            QMapString tempMap;

                            QJsonValue JsonValue = value.toString();
                            tempMap["text"] = JsonValue.toString();

                            listJsonkepMap.append(tempMap);
                        }
                        else if (value.isDouble())
                        {
                            QMapString tempMap;

                            QJsonValue JsonValue = value.toDouble();


                            tempMap["text"] = QString::number(JsonValue.toDouble(), 'f', 0);
                            listJsonkepMap.append(tempMap);
                        }
                    }

                    return true;
                }
                else if(jsonValue.isObject())
                {
                    //特殊情况，可能名称相同，先对象后数组，如果再不是，直接返回false
                    QJsonObject jsonobject = jsonValue.toObject();
                    //再取一次
                    if(jsonobject.contains(arrayKey))
                    {
                        QJsonValue jsonvalue = jsonobject.value(arrayKey);
                        //进行判断，是否是数组
                        if(jsonvalue.isArray())
                        {
                            QJsonArray array = jsonvalue.toArray();

                            for(int i = 0; i < array.size(); i++)
                            {
                                QJsonValue value =  array.at(i);

                                if(value.isObject())
                                {
                                    QJsonObject object = array.at(i).toObject();

                                    QStringList keyList = jsonkey.keys();

                                    QMapString tempMap;
                                    for(QString key : keyList)
                                    {
                                        if(object.contains(key))
                                        {
                                            QJsonValue value = object.value(key);
                                            QVariant varVariant = value.toVariant();

                                            tempMap[key] = varVariant.toString();
                                        }
                                    }

                                    listJsonkepMap.append(tempMap);
                                }
                            }
                        }
                    }

                    return true;
                }
                return false;
            }
        }
    }

    return false;
}

void HelpClass::parseJsonData(QByteArray json, QMap<QString, QVariant> &jsonData, QStringList rootkey)
{

    parseJsonData(QJsonDocument::fromJson(json), jsonData, rootkey);
}

void HelpClass::parseJsonData(QJsonDocument document, QMap<QString, QVariant> &jsonData, QStringList rootkey)
{

    if(document.isObject())
    {
        QJsonObject jsonObject = document.object();

        for (auto jsonKey:rootkey)
        {
            if(jsonObject.contains(jsonKey))
            {
               QJsonValue jsonValue =  jsonObject.value(jsonKey);

               if(jsonValue.isObject())
               {
                    parseJsonObject(jsonValue.toObject(),jsonData,rootkey);
               }
               else if(jsonValue.isArray())
               {
                    parseJsonArray(jsonValue.toArray(),jsonData, rootkey, jsonKey);
               }
               else {
                   //普通数据
                   jsonData[jsonKey] = jsonValue.toVariant();
               }
            }
        }

    }
    else if(document.isArray())
    {

    }
    else if(document.isNull() || document.isEmpty())
    {
        return;
    }
}

void HelpClass::parseJsonObject(QJsonObject jsonObject, QMap<QString, QVariant> &jsonData, QStringList parseKey)
{
    if(jsonObject.isEmpty())
    {
        qInfo()<<"jsonObejct is Empty";
        return;
    }

    for(auto key : parseKey)
    {
        if(jsonObject.contains(key))
        {
            QJsonValue value = jsonObject.value(key);

            if(value.isObject())
            {
                 parseJsonObject(value.toObject(),jsonData,parseKey);
            }
            else if(value.isArray())
            {
                 parseJsonArray(value.toArray(), jsonData, parseKey, key);
            }
            else {
                //普通数据
                QVariantList tempMap;
                QStringList keyList= jsonData.keys();

                if(keyList.contains(key)){

                    QVariantList tempVar =  jsonData[key].toList();
                    tempMap = tempVar;
                }

                QVariant data =value.toVariant();
                if(data.isNull() || data == "")
                {
                    data = "None";
                }
                tempMap.append(data);

                jsonData[key] = tempMap;
            }
        }
    }
}

void HelpClass::parseJsonArray(QJsonArray jsonArray, QMap<QString, QVariant> &jsonData, QStringList parseKey, QString arraykey)
{
    int arraySize = jsonArray.size();

    for(int i = 0; i < arraySize; i++)
    {
        if(jsonArray[i].isObject())
        {
            QJsonObject jsonObject = jsonArray[i].toObject();
            parseJsonObject(jsonObject, jsonData, parseKey);
        }
        else if(jsonArray[i].isArray())
        {
            QJsonArray jsonarray= jsonArray[i].toArray();
            parseJsonArray(jsonarray,jsonData,parseKey,arraykey);
        }
        else
        {
		
            //普通数据
            QStringList keyList= jsonData.keys();
			QVariantList tempMap;
			//这里取出来是有bug存在
            if(keyList.contains(arraykey)){

                QVariantList tempVar =  jsonData[arraykey].toList();
                tempMap = tempVar;
            }

            QVariant data =jsonArray[i].toVariant();

            if(data.isNull() || data == "")
            {
                data = "None";
            }
            tempMap.append(data);

            jsonData[arraykey] = tempMap;
        }
    }
}

QPair<QMap<QString, int>, QMap<QString,  QList<QMapString>> > HelpClass::jsonAllFormatsParsePair(QJsonDocument jsonDocument, QStringList parseJsonKey, QStringList rootKey)
{
    QPair<QMap<QString, int>, QMap<QString,  QList<QMapString>> > pairMapJsonData;
    QJsonObject object = jsonDocument.object();
    // 需要返回的类型参数
    QMap<QString, QList<QMapString> > mapList;

    //接收解析后的json格式
    QList<TypedefAlias::QMapStringMap> listMapString = parseJsonObject(object, parseJsonKey ,rootKey);

    // qInfo()<<"listMapString"<<listMapString;
    //下面进行格式化融合
    int listMapLength = listMapString.length();
    //qInfo()<<"listMapLength"<<listMapLength;
    if(listMapLength > 0)
    {
        QStringList jsonkeys = listMapString.at(0).keys();

        if(jsonkeys.length() > 0)
        {
            QString oldKey = jsonkeys[0];
            //qInfo()<<"oldKey = "<<oldKey;
            //用于记录json格式拆解后的QMapStringMap中key对应的QMapString
            QList<QMapString> mapStringList;

            for(int i =  0;  i < listMapLength; i++)
            {
                QMapString tempMapString;
                QStringList keyList = listMapString.at(i).keys();

                if(keyList.contains(oldKey))
                {
                    tempMapString = listMapString.at(i)[oldKey];
                    mapStringList.append(tempMapString);
                }
                else {
                    mapList[oldKey] = mapStringList;
                    mapStringList.clear();
                    if(keyList.length() >= 1)
                    {
                        oldKey = keyList[0];
                    }
                    tempMapString = listMapString.at(i)[oldKey];
                    mapStringList.append(tempMapString);
                }
            }
            //最后退出再赋值一次
            mapList[oldKey] = mapStringList;
            mapStringList.clear();
            //qInfo() << "mapList = " << mapList;
        }
    }

    QStringList jsonkeys = mapList.keys();
    QMap<QString, int> lengthKey;
    for(auto key:rootKey)
    {
        int keyindex = 0;

        QString keyMapList ; //判断mapList中的key是否带数字又只有一个数据
        for(auto jsonkey:jsonkeys)
        {
            QString tmepKey = jsonkey;
            //去掉数字，计算每个关键词的个数
            midDigits(jsonkey);
            if(jsonkey == key)
            {
                keyMapList = tmepKey;
                keyindex++;
            }
        }

        //如果keyindex = 1并且key后面还包含数字，就必须追加一个伪数据到达2
        if(keyindex == 1)
        {
            //返回true代表包含数字
            if(isStrContainsNumbers(keyMapList))
            {
                QList<QMapString> &mapListdata = mapList[keyMapList];
                QMapString a;
                a["abcTempData"]="a";
                mapListdata.append(a);
                keyindex++;

            }
        }


        lengthKey[key] = keyindex;
    }

    pairMapJsonData.first = lengthKey;
    pairMapJsonData.second = mapList;

    //qInfo() << "maoList = " << mapList;

    return pairMapJsonData;
}

QMap<QString, QStringList> HelpClass::jsonAllFormatsParseMap(QJsonDocument jsonDocument, QStringList parseJsonKey, QStringList rootKey)
{

    QPair<QMap<QString, int>, QMap<QString,  QList<QMapString>> > pairMapJsonData = jsonAllFormatsParsePair(jsonDocument, parseJsonKey, rootKey);

    auto lengthKey = pairMapJsonData.first;
    auto mapList = pairMapJsonData.second;

    QMap<QString, QStringList> mapStringList;
    for(auto key:rootKey)
    {
        int value  = lengthKey[key];
        if(value > 1)
        {
            for(int i = 0; i < value; i++)
            {
                auto jsonMapkey = key+QString::number(i);
                QList<QMapString> tempMapString = mapList[jsonMapkey];

                for(auto key:parseJsonKey)
                {
                    QStringList strListValue;
                    for(auto mapString: tempMapString)
                    {
                        if(mapString.contains(key))
                        {
                            QString valueStr= mapString[key];
                            //![2019年9月18日 14:28:49 更新下面语句，如果为空，自动补None，以防顺序被打乱]
                            if(valueStr.isEmpty())
                            {
                                valueStr = "None";
                            }
                            //![2019年9月18日 14:28:49 更新下面语句，如果为空，自动补None，以防顺序被打乱]
                            //!
                            strListValue.append(valueStr);
                        }
                    }
                    if(mapStringList.contains(key) && !strListValue.isEmpty())
                    {
                        auto & strlistvalue =  mapStringList[key];
                        for(auto str:strListValue)
                        {
                            strlistvalue.append(str);
                        }
                    }
                    else {
                        if (!strListValue.isEmpty())
                        {
                            mapStringList[key] = strListValue;
                        }
                    }
                }
            }

        }
        else if(value == 1)
        {
            QList<QMapString> tempMapString = mapList[key];

            for(auto keys:parseJsonKey)
            {
                QStringList strListValue;
                for(auto mapString: tempMapString)
                {
                    if(mapString.contains(keys))
                    {
                        QString valueStr= mapString[keys];

                        //![2019年9月18日 14:28:49 更新下面语句，如果为空，自动补None，以防顺序被打乱]
                        if(valueStr.isEmpty())
                        {
                            valueStr = "None";
                        }
                        //![2019年9月18日 14:28:49 更新下面语句，如果为空，自动补None，以防顺序被打乱]
                        //!
                        strListValue.append(valueStr);
                    }
                }
                if(!strListValue.isEmpty())
                {
                    mapStringList[keys] = strListValue;
                }
            }

        }
    }

    //qInfo() << "mapString" << mapStringList;
    return mapStringList;
}

QMap<QString, QList<QList<QMapString>> > HelpClass::jsonAllFormatsParseMap_II(QJsonDocument jsonDocument, QStringList parseJsonKey, QStringList rootKey)
{
    //返回值
    QMap<QString, QList<QList<QMapString> > > mapStringListMap;

    //! 解析对应的json数据
    QPair<QMap<QString, int>, QMap<QString,  QList<QMapString>> > pairMapJsonData = jsonAllFormatsParsePair(jsonDocument, parseJsonKey, rootKey);

    auto lengthKey = pairMapJsonData.first;
    auto mapList = pairMapJsonData.second;


    //!下面进行整合
    for(auto key:rootKey)
    {

        //此变量会和mapStringListMap返回值变量交互的，用于保存后返回
        QList<QMapString> listMapResult;

        //根据值的大小来进行不同的数据解析，
        int value  = lengthKey[key];
        //大于1的后缀的需要加数字1，2，3，4
        if(value > 1)
        {
            QList<QList<QMapString> > listListMap;

            for(int i = 0; i < value; i++)
            {
                listMapResult.clear();
                auto jsonMapkey = key+QString::number(i);
                QList<QMapString> tempMapString = mapList[jsonMapkey];

                //!开始遍历拿到的相关key的数据
                for(auto mapString:tempMapString)
                {
                    listMapResult.append(mapString);
                }
                listListMap.append(listMapResult);

            }
            mapStringListMap[key] = listListMap;


            //! 直接key 对应追加好的数据
            // mapStringListMap[key] = listMapResult;

        }
        //!直接解析，判断，不需要加数字，能进入到这里说明就不是数组类型，都是单个变量
        //! 不用考虑一个变量对应多个值的情况，这种情况只会在上面处理
        else if(value == 1)
        {
            QList<QList<QMapString>> listListMap;
            QString keystr = "Root";
            //!获取根key相关的所有数据,这个里面可能含有根key，所以需要遍历一遍，以防万一
            QList<QMapString> listMapstrTemp = mapList[key];

            //!开始遍历拿到的相关key的数据
            for(auto mapString:listMapstrTemp)
            {
                listMapResult.append(mapString);
            }
            listListMap.append(listMapResult);
            mapStringListMap[keystr] = listListMap;
        }
    }

    //qInfo() << "mapStringListMap = " << mapStringListMap;

    return mapStringListMap;
}

QString HelpClass::getUuid()
{
    return  QUuid::createUuid().toString();
}

QString HelpClass::getCurrentAppDataDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

QString HelpClass::getCurrentTempDataDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::TempLocation);
}

QString HelpClass::getCurrentFontDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::FontsLocation);

}

bool HelpClass::writeDataToFiles(QList<QMap<QString, QStringList> > mapList, QString Path)
{
    if(Path.isEmpty())
    {
        Path = getCurrentTempDataDir()+"/Data";
    }

    if(!isDirExist(Path))
    {
        return false;
    }

    Path = Path +"/"+AgencyFileName;

    //保险起见先把原先的文件删除再重写
    removeFile(Path);

    QFile * file = new QFile(Path);

    //默认只写，不会追加前面已有的数据，而是覆盖前面的数据
    if(!file->open(QIODevice::WriteOnly)){
        qInfo()<<QString("%1 file WriteOnly open failed~").arg(Path);
        return false;
    }
    QDataStream  dataStream(file);
    dataStream.setVersion(QDataStream::Qt_5_12);
    QList<QMap<QString, QStringList> >::ConstIterator iter = mapList.constBegin();

    while(iter != mapList.constEnd())
    {
        dataStream<<(*iter);

        iter++;
    }

    file->close();

    delete file;


    return true;
}

bool HelpClass::writeDataToFile(QList<QMap<QString, QString> > mapList, QString Path)
{
    if(Path.isEmpty())
    {
        Path = getCurrentTempDataDir()+"/Data";
		Path = Path + "/" + AgencyFileName;
    }

	QFileInfo fileInfo(Path);
	QString dirPath = fileInfo.path();
	if (dirPath.isEmpty())
	{
		dirPath = Path;
	}

    if(!isDirExist(dirPath))
    {
        return false;
    }


    //保险起见先把原先的文件删除再重写
    removeFile(Path);

    QFile * file = new QFile(Path);

    //默认只写，不会追加前面已有的数据，而是覆盖前面的数据
    if(!file->open(QIODevice::WriteOnly)){
        qInfo()<<QString("%1 file WriteOnly open failed~").arg(Path);
        return false;
    }
    QDataStream  dataStream(file);
    dataStream.setVersion(QDataStream::Qt_5_12);
    QList<QMap<QString, QString> >::ConstIterator iter = mapList.constBegin();

    while(iter != mapList.constEnd())
    {
        dataStream<<(*iter);

        iter++;
    }

    file->close();

    delete file;


    return true;
}

QList<QMap<QString, QStringList> > HelpClass::ReadDataFromFiles(QString Path)
{

    if(Path.isEmpty())
    {
        Path = getCurrentTempDataDir()+"/Data/";
    }

    Path = Path +"/"+AgencyFileName;
    QFile * file = new QFile(Path);
    QList<QMap<QString, QStringList> > mapList;
    //默认只写，不会追加前面已有的数据，而是覆盖前面的数据
    if(!file->open(QIODevice::ReadOnly)){
        qCritical()<<QString("%1 file ReadOnly open failed~").arg(Path);
        return mapList;
    }

    QDataStream  dataStream(file);
    dataStream.setVersion(QDataStream::Qt_5_12);

    while(!dataStream.atEnd())
    {
        QMap<QString, QStringList>  mapData;
        dataStream>>mapData;

        mapList.append(mapData);
    }
    file->close();

    delete file;
	file = nullptr;

    return mapList;
}

QList<QMap<QString, QString> > HelpClass::ReadDataFromFile(QString Path)
{
    if(Path.isEmpty())
    {
        Path = getCurrentTempDataDir()+"/Data/";

		Path = Path + "/" + AgencyFileName;
    }

    QFile * file = new QFile(Path);
    QList<QMap<QString, QString> > mapList;
    //默认只写，不会追加前面已有的数据，而是覆盖前面的数据
    if(!file->open(QIODevice::ReadOnly)){
        qCritical()<<QString("%1 file ReadOnly open failed~").arg(Path);
        return mapList;
    }

    QDataStream  dataStream(file);
    dataStream.setVersion(QDataStream::Qt_5_12);

    while(!dataStream.atEnd())
    {
        QMap<QString, QString>  mapData;
		
        dataStream>>mapData;

		if (!mapData.isEmpty())
		{
			mapList.append(mapData);
		}
        
    }
    file->close();

    delete file;
	file = nullptr;

    return mapList;
}

QByteArray HelpClass::encryption(const QByteArray plaintextStr)
{
    QByteArray ciphertext = plaintextStr.toBase64();

    return ciphertext;
}

QByteArray HelpClass::encryption(const QString plaintextStr)
{
    QByteArray plaintextByte = plaintextStr.toLocal8Bit();

    return encryption(plaintextByte);
}

QByteArray HelpClass::Deciphering(const QByteArray ciphertext)
{
    QByteArray plaintext = QByteArray::fromBase64(ciphertext);
    return plaintext;
}

QByteArray HelpClass::getXorEncryptDecrypt(const QByteArray &str, const QByteArray &key)
{
    QByteArray bs = str;

    for(int i=0; i<bs.size(); i++){
        for(int j  = 0; j < key.size(); j++)
            bs[i] = bs[i] ^ key[j];
    }

    return bs;
}

void HelpClass::midDigits(QString &src)
{
    //  int digitsNum = 0;
    for(int i = 0; i < 10; i++)
    {
        //检索是否含有数字，并返回该位置
        int index = src.lastIndexOf(QString::number(i));
        if(index != -1)
        {
            src.remove(index, 1);
            i = -1;
        }
    }

    //src.chop(digitsNum);
}

QList<TypedefAlias::QMapStringMap> HelpClass::parseJsonObject(QJsonObject jsonObject, QStringList mapSubKey, QStringList keyrootList, QString keyroot)
{
    static QList<TypedefAlias::QMapStringMap> mapList;
    QStringList mapKeyList = mapSubKey;
    QMapString tempMap;
    TypedefAlias::QMapStringMap tempMappString;

    for(auto key:mapKeyList)
    {
        if(jsonObject.contains(key))
        {
            QJsonValue value = jsonObject.value(key);

            if(keyrootList.contains(key))
            {
                keyroot = key;
            }
            if(value.isArray())
            {
                mapList.append(parseJsonArray(value.toArray(), keyroot ,mapSubKey, keyrootList));
            }
            else if(value.isObject())
            {
                mapList.append(parseJsonObject(value.toObject(), mapSubKey,keyrootList, keyroot));
            }
            else
            {
                QVariant var = value.toVariant();
                tempMap[key] = var.toString();
                tempMappString[keyroot] = tempMap;
            }
        }
    }
    if(!tempMappString.isEmpty())
    {
        mapList.append(tempMappString);
    }


    QList<TypedefAlias::QMapStringMap> tempMapList = mapList;
    mapList.clear();
    //    for (auto mapList : tempMapList)
    //    {
    //        qInfo() << "mapList = " << mapList;
    //    }
    return tempMapList;
}

QList<TypedefAlias::QMapStringMap> HelpClass::parseJsonArray(QJsonArray jsonArray, QString key, QStringList mapSubKey, QStringList keyrootList)
{
    static QList<TypedefAlias::QMapStringMap> mapList;
    int nSize = jsonArray.size();
    QMapString mapString;
    TypedefAlias::QMapStringMap tempMappString;

    static int enterCount = 0;
    enterCount++;
    //去掉Key后面的数字，还原原始Key
    midDigits(key);
    static int index = 0;
    for (int i = 0; i < nSize; ++i)
    {
        QJsonValue value = jsonArray.at(i);

        //等于1表示只有根的情况下才进行后面的数字添加
        if(enterCount == 1)
        {
            index = i;
        }

        if(value.isObject())
        {
            QJsonObject object = value.toObject();
            mapList.append(parseJsonObject(object, mapSubKey, keyrootList, key+QString::number(index)));
        }
        else if(value.isArray())
        {
            QJsonArray array = value.toArray();

            mapList.append(parseJsonArray(array, key+QString::number(index), mapSubKey,keyrootList));
        }
        else
        {
            QVariant var = value.toVariant();
            mapString[key] = var.toString();
            tempMappString[key+QString::number(i)] = mapString;
        }
    }

    if(!tempMappString.isEmpty())
    {
        mapList.append(tempMappString);
    }

    QList<TypedefAlias::QMapStringMap> tempMapList = mapList;
    mapList.clear();
    enterCount--;
    return tempMapList;
}

void HelpClass::writeSettingFile(QMap<QString, QString> mapSettingData, QString groupName, QString filePath, bool isEncrypt)
{
    QSettings settings(filePath, QSettings::IniFormat);

    QStringList mapList = mapSettingData.keys();

    settings.beginGroup(groupName);
    for(QString key : mapList)
    {
        QString value = mapSettingData.value(key);
        if(isEncrypt)
        {
            QByteArray encryptionText = aes128Encryption(AESKEY, value.toUtf8());

            QByteArray aes128EncryHex = encryptionText.toHex();
            encryptionText = GetEncrypt(aes128EncryHex, DefaultKEY);
            settings.setValue(key,encryptionText);
        }
        else
        {
            settings.setValue(key, value);
        }
    }

    settings.endGroup();
}

QMap<QString, QString> HelpClass::readSettingFile(QString groupName, QString filePath, bool isDecrypt)
{
    QMap<QString, QString> map;
    QSettings settings(filePath,QSettings::IniFormat);

    settings.beginGroup(groupName);
    QStringList keyList = settings.allKeys();

    for(QString key :keyList)
    {
        QString value;
        if(isDecrypt)
        {
            QVariant keyValue = settings.value(key);
            QByteArray valueData = keyValue.toByteArray();
            valueData = GetDecrypt(valueData, DefaultKEY);

            value = aes128Decrypt(AESKEY, QByteArray::fromHex(valueData));
        }
        else
        {
            value = settings.value(key).toString();
        }
        map.insert(key, value);
    }

    settings.endGroup();
    return map;
}
#ifndef QMLAPP
void HelpClass::ToolTipmessage(QWidget *parent, QString message, QPoint point)
{
    ToolTip tip(parent);

    //显示时间为1.5秒
    tip.setToolTipDelay(1500);

    QEventLoop loop;

    if(!point.isNull())
    {
        tip.showToolMessage(point, message);
    }
    else
    {
        int toolWidtg = tip.getToolTipWidth(message);
        tip.setAnimationPopupPosition(QPoint((parent->width() - toolWidtg)/2, -60), QPoint((parent->width()- toolWidtg)/2, 15));

        tip.showToolMessage(message);
    }


    connect(&tip, &ToolTip::sighide, &loop, &QEventLoop::quit);

    loop.exec();
}

int HelpClass::getFontWidth(QString text)
{
    QFontMetrics fontMetrics(QFont("Microsoft Yahei",12));
    return fontMetrics.width(text);
}

void HelpClass::showDialogText(QWidget *parent, QString showText, bool free)
{
    //释放label指针
    if(free)
    {
        if(m_label)
        {
            //如果有父对象走Qt释放如下
            if(m_label->parent())
            {
                m_label->deleteLater();
            }
            else
            {
                delete m_label;
            }
            m_label = nullptr;
        }
        return;
    }

    if(m_label == nullptr)
    {
        m_label = new QLabel(parent);
        int fontwidget = getFontWidth(showText)+10;

        int labelX = (parent->width() - fontwidget)/2;
        int labelY = (parent->height()/2)+10;

        m_label->setGeometry(labelX, labelY, fontwidget, 50);
        m_label->setStyleSheet("   background-color:#FFFFFF; \
                               border-radius:2px 0px 0px 2px; \
                border-bottom:0px solid rgba(234,234,234,1); \
        font-size:15px; \
        font-family:Microsoft YaHei;  \
color:#333333;");
        m_label->setText(showText);
        m_label->raise();
        m_label->show();
    }
}

void HelpClass::setGraphicsEffect(QWidget *parent)
{
    FGraphicsEffect * bodyShadow = new  FGraphicsEffect(parent);
    bodyShadow->setBlurRadius(10.0);
    bodyShadow->setDistance(4.0);
    bodyShadow->setColor(QColor(0, 0, 0, 20));
    parent->setGraphicsEffect(bodyShadow);
}
#endif



void HelpClass::copyText(QString text)
{
    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    clipboard->setText(text);	 //设置剪贴板内容
}

QString HelpClass::size(qint64 bytes)
{
    QString strUnit;
    double dSize = bytes * 1.0;
    if (dSize <= 0)
    {
        dSize = 0.0;
    }
    else if (dSize < 1024)
    {
        strUnit = "Bytes";
    }
    else if (dSize < 1024 * 1024)
    {
        dSize /= 1024;
        strUnit = "KB";
    }
    else if (dSize < 1024 * 1024 * 1024)
    {
        dSize /= (1024 * 1024);
        strUnit = "MB";
    }
    else
    {
        dSize /= (1024 * 1024 * 1024);
        strUnit = "GB";
    }

    return QString("%1 %2").arg(QString::number(dSize, 'f', 2)).arg(strUnit);
}

QString HelpClass::speed(double speed)
{
    QString strUnit;
    if (speed <= 0)
    {
        speed = 0;
        strUnit = "Bytes/S";
    }
    else if (speed < 1024)
    {
        strUnit = "Bytes/S";
    }
    else if (speed < 1024 * 1024)
    {
        speed /= 1024;
        strUnit = "KB/S";
    }
    else if (speed < 1024 * 1024 * 1024)
    {
        speed /= (1024 * 1024);
        strUnit = "MB/S";
    }
    else
    {
        speed /= (1024 * 1024 * 1024);
        strUnit = "GB/S";
    }

    QString strSpeed = QString::number(speed, 'f', 2);
    return QString("%1 %2").arg(strSpeed).arg(strUnit);
}

QString HelpClass::timeFormat(qint64  seconds)
{
    QString strValue;
    QString strSpacing(" ");
    if (seconds <= 0)
    {
        strValue = QString("%1s").arg(0);
    }
    else if (seconds < 60)
    {
        strValue = QString("%1s").arg(seconds);
    }
    else if (seconds < 60 * 60)
    {
        int nMinute = seconds / 60;
        int nSecond = seconds - nMinute * 60;

        strValue = QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else if (seconds < 60 * 60 * 24)
    {
        int nHour = seconds / (60 * 60);
        int nMinute = (seconds - nHour * 60 * 60) / 60;
        int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else
    {
        int nDay = seconds / (60 * 60 * 24);
        int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
        int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
        int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1d").arg(nDay);

        if (nHour > 0)
            strValue += strSpacing + QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }

    return strValue;
}

TypedefAlias::QListMapStringMap HelpClass::xmlReadParse(TypedefAlias::QMapStringMap  &xmlMapPara, const QString &rootElement, const QByteArray &xmlDataStream)
{
    QXmlStreamReader xmlReader(xmlDataStream);  //存放数据流xml
    TypedefAlias::QListMapStringMap listMap;  //将要返回的 ListMap 值
    if(xmlReader.readNextStartElement())  //读取第一个元素
    {
        QString strName = xmlReader.name().toString();  //读取到的元素名称
        //qInfo()<<"读取到的第一个xml根元素."<<strName;

        //! [0]
        if (strName== rootElement)   // 获取根元素, 判断根元素是否正确，不正确直接返回
        {
            listMap =  whileReadXml(xmlReader, xmlMapPara);

        } //end ![0]
        else
        {
            xmlReader.raiseError("XML file format error.");

            QString xmlError = xmlErrorString(xmlReader);

            TypedefAlias::QMapString xmlMapPara;
            xmlMapPara["XMLError"] = xmlError;
            TypedefAlias::QMapStringMap mapStrMap;
            mapStrMap["ERROR"]= xmlMapPara;
            listMap.append(mapStrMap);

        }

    }

    return listMap;
}

TypedefAlias::QListMapStringMap HelpClass::xmlReadParse(TypedefAlias::QMapStringMap  &xmlMapPara, const QString & rootElement, const QString &xmlFile)
{
    QFile file(xmlFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {  // 以只读模式打开
        qWarning()<< QString("Cannot read file %1(%2).").arg(xmlFile).arg(file.errorString());
        TypedefAlias::QListMapStringMap map;
        map.clear(); //返回一个空的listMap
        return  map;
    }

    QByteArray xmlStream = file.readAll();

    return xmlReadParse(xmlMapPara, rootElement, xmlStream);
}

TypedefAlias::QListMapStringMap HelpClass::xmlReadParse(TypedefAlias::QMapString &xmlMapPara, const QStringList &parseElement, const QString &rootElement, const QByteArray &xmlDataStream, const QStringList &parseAttributes)
{
    QXmlStreamReader xmlReader(xmlDataStream);  //存放数据流xml
    TypedefAlias::QListMapStringMap listMap;  //将要返回的 ListMap 值

    if(xmlReader.readNextStartElement())  //读取第一个元素
    {
        QString strName = xmlReader.name().toString();  //读取到的元素名称
        //qInfo()<<"读取到的第一个xml根元素."<<strName;

        //! [0]
        if (strName== rootElement)   // 获取根元素, 判断根元素是否正确，不正确直接返回
        {
            listMap =  whileReadXml(xmlReader, parseElement, xmlMapPara,parseAttributes);

        } //end ![0]
        else
        {
            xmlReader.raiseError("XML file format error.");

            QString xmlError = xmlErrorString(xmlReader);

            xmlMapPara["XMLError"] = xmlError;
            TypedefAlias::QMapStringMap mapStrMap;
            mapStrMap["ERROR"]= xmlMapPara;
            listMap.append(mapStrMap);
        }

    }

    return listMap;
}

TypedefAlias::QListHashStringMap HelpClass::xmlReadParse(TypedefAlias::QHashString &xmlMapPara, const QStringList &parseElement, const QString &rootElement, const QByteArray & xmlDataStream)
{
    QXmlStreamReader xmlReader(xmlDataStream);  //存放数据流xml
    TypedefAlias::QListHashStringMap listMap;  //将要返回的 ListMap 值

    if(xmlReader.readNextStartElement())  //读取第一个元素
    {
        QString strName = xmlReader.name().toString();  //读取到的元素名称
        //qInfo()<<"读取到的第一个xml根元素."<<strName;

        //! [0]
        if (strName== rootElement)   // 获取根元素, 判断根元素是否正确，不正确直接返回
        {
            listMap =  whileReadXml(xmlReader, parseElement, xmlMapPara);

        } //end ![0]
        else
        {
            xmlReader.raiseError("XML file format error.");

            QString xmlError = xmlErrorString(xmlReader);

            xmlMapPara["XMLError"] = xmlError;
            TypedefAlias::QHashStringMap mapStrMap;
            mapStrMap["ERROR"]= xmlMapPara;
            listMap.append(mapStrMap);
        }

    }

    return listMap;

}

TypedefAlias::QListMapStringMap HelpClass::xmlReadParse(TypedefAlias::QMapString &xmlMapPara, const QStringList &parseElement, const QString &rootElement, const QString &xmlFile)
{

    QFile file(xmlFile);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {  // 以只读模式打开
        qWarning()<< QString("Cannot read file %1(%2).").arg(xmlFile).arg(file.errorString());
        TypedefAlias::QListMapStringMap map;
        map.clear(); //返回一个空的listMap
        return  map;
    }

    QByteArray xmlStream = file.readAll();

    return  xmlReadParse(xmlMapPara, parseElement,rootElement,xmlStream);
}

TypedefAlias::QListMapStringMap HelpClass::whileReadXml(QXmlStreamReader &xml,  TypedefAlias::QMapStringMap &mapStringMap)
{
    TypedefAlias::QListMapStringMap listMapString;
    //![1] 循环读取父元素
    while (xml.readNextStartElement())
    {

        bool isRead = false;  //是否读到元素，没有读到则跳过当前元素，直接读取下一个元素
        QString strName = xml.name().toString();  //读取到的元素标签名称
        QStringList xmlKeys = mapStringMap.keys();
        TypedefAlias::QMapStringMap tempMapStringMap;
        //循环匹配对应的父元素标签
        for(auto key:xmlKeys)
        {
            if(strName ==  key)  //如果相等，说明xml的父元素标签是一致的，接下来就要取出子元素文本了
            {
                //取出子元素
                isRead = true;

                whileReadXml(xml, mapStringMap[key]);

                tempMapStringMap[key] = mapStringMap[key];
            }
        }

        listMapString.append(tempMapStringMap);

        if(!isRead)
        {
            xml.skipCurrentElement();
        }

    }//end ![1]

    return listMapString;
}

void HelpClass::whileReadXml(QXmlStreamReader &xml, TypedefAlias::QMapString &mapString)
{

    //这里开始循环读取子元素
    while (xml.readNextStartElement())
    {
        QStringList  strList = mapString.keys();
        QString strName = xml.name().toString();  //读取到的元素标签名称,子元素了
        bool isRead = false;  //是否读到元素，没有读到则跳过当前元素，直接读取下一个元素

        for(auto xmlKey:strList)
        {
            if(xmlKey == strName)
            {
                isRead = true;
                QString xmlElement = xml.readElementText();
                mapString[strName] = xmlElement;
            }
        }

        //跳过当前的元素
        if(!isRead)
        {
            xml.skipCurrentElement();
        }
    }
}

TypedefAlias::QListMapStringMap HelpClass::whileReadXml(QXmlStreamReader &xml,  const QStringList &stringElement, QMapString &mapString, const QStringList &parseAttributes)
{

    //因为递归原因，如果不用静态static，每次递归都会导致listMap清空，中间赋值listMap总是只有一个，返回自然是一个空值，记录不了上次递归所存储的值
    static TypedefAlias::QListMapStringMap listMap;

    //需要返回的lsitMap的复制品，返回前需清空listMap，否则每次都会记录前面的数据，会导致混乱，一次递归就只返回记录一次值
    TypedefAlias::QListMapStringMap tempListMap;
    //保存将要记录的xml中的元素的key和value，过滤不需要的元素
    QMapString tempMapString;

    //这里开始循环读取父子元素 ![0]
    while (xml.readNextStartElement())
    {

        QString strName = xml.name().toString();  //读取到的元素标签名称,可能是父元素，也可能是子元素了
        TypedefAlias::QMapStringMap mapStringMap;

        bool isRead = false;  //是否读到的是一个父元素标签
        for(auto xmlkey:stringElement)
        {
            //如果相等继续读取，进行递归操作读取子元素
            if(xmlkey == strName)
            {
                isRead = true;
                //进行递归读取，一旦发现有父元素节点，就要递归读取子节点
                QMapString tempMapString = mapString;

                //递归接收获取到listMap数据，前面的static 变量也会对应更改，返回时会清空，下次调用，不会有前面的数据
                listMap = whileReadXml(xml, stringElement, tempMapString, parseAttributes);

                //判断获取到的临时Map是否是空值，非空就赋值
                if(!tempMapString.isEmpty()){
                    QStringList mapValue = tempMapString.values();

                    bool isEmpty = true;
                    for(auto strvalue: mapValue)
                    {
                        //如果有一个不空，则写入返回，如果都空，则不返回，过滤掉
                        if(!strvalue.isEmpty())
                        {
                            isEmpty = false;
                        }
                    }
                    if(!isEmpty)
                    {
                        mapStringMap[strName] = tempMapString;
                    }

                }

                if(!mapStringMap.isEmpty())
                {
                    listMap.append(mapStringMap);
                }

            }
        }

        //如果为false，说明没有读到对应的父元素，应该解析下面的子元素
        if(! isRead)
        {
            //whileReadXml(xml, mapString);
            QStringList  strList = mapString.keys();

            bool aisRead = false;  //是否读到元素，没有读到则跳过当前元素，直接读取下一个元素

            for(auto xmlKey:strList)
            {
                if(xmlKey == strName)
                {
                    aisRead = true;

                    //如果非空说明需要解析对应的属性
                    if (!parseAttributes.isEmpty())
                    {
                        for (auto attributes : parseAttributes)
                        {
                            //qInfo() << "attributes" << attributes;
                            QXmlStreamAttributes xmlAttributes = xml.attributes();
                            if (xmlAttributes.hasAttribute(attributes))
                            {
                                QString AttributesValue = xmlAttributes.value(attributes).toString();
                                QString xmlElement = xml.readElementText();
                                tempMapString[strName+"@|$"+attributes+"@|$"+AttributesValue] = xmlElement;
                            }
                        }
                    }

                    QString xmlElement = xml.readElementText();
                    if (!xmlElement.isEmpty())
                    {
                        tempMapString[strName] = xmlElement;
                    }




                }
            }
            //跳过结束下面的赋值
            if(!aisRead)
            {
                xml.skipCurrentElement();
            }
        }
    }// end ![0]

    //进行复制品赋值，准备清空listMap
    tempListMap = listMap;
    //清空listMap,每次返回都需要清空
    listMap.clear();
    //情况mapstring的引用，准备重新赋值新的过滤好的数据
    mapString.clear();
    //赋值过滤好的Xml元素属性数据
    mapString = tempMapString;

    //返回复制品的listMap
    return tempListMap;
}

TypedefAlias::QListHashStringMap HelpClass::whileReadXml(QXmlStreamReader &xml, const QStringList &stringElement, TypedefAlias::QHashString &mapString)
{
    //因为递归原因，如果不用静态static，每次递归都会导致listMap清空，中间赋值listMap总是只有一个，返回自然是一个空值，记录不了上次递归所存储的值
    static TypedefAlias::QListHashStringMap listMap;

    //需要返回的lsitMap的复制品，返回前需清空listMap，否则每次都会记录前面的数据，会导致混乱，一次递归就只返回记录一次值
    TypedefAlias::QListHashStringMap tempListMap;
    //保存将要记录的xml中的元素的key和value，过滤不需要的元素
    TypedefAlias::QHashString tempMapString;

    //这里开始循环读取父子元素 ![0]
    while (xml.readNextStartElement())
    {

        QString strName = xml.name().toString();  //读取到的元素标签名称,可能是父元素，也可能是子元素了
        TypedefAlias::QHashStringMap mapStringMap;

        bool isRead = false;  //是否读到的是一个父元素标签
        for(auto xmlkey:stringElement)
        {
            //如果相等继续读取，进行递归操作读取子元素
            if(xmlkey == strName)
            {
                isRead = true;
                //进行递归读取，一旦发现有父元素节点，就要递归读取子节点
                TypedefAlias::QHashString tempMapString = mapString;

                //递归接收获取到listMap数据，前面的static 变量也会对应更改，返回时会清空，下次调用，不会有前面的数据
                listMap = whileReadXml(xml, stringElement, tempMapString);

                //判断获取到的临时Map是否是空值，非空就赋值
                if(!tempMapString.isEmpty()){
                    QStringList mapValue = tempMapString.values();

                    bool isEmpty = true;
                    for(auto strvalue: mapValue)
                    {
                        //如果有一个不空，则写入返回，如果都空，则不返回，过滤掉
                        if(!strvalue.isEmpty())
                        {
                            isEmpty = false;
                        }
                    }
                    if(!isEmpty)
                    {
                        mapStringMap[strName] = tempMapString;
                    }

                }

                if(!mapStringMap.isEmpty())
                {
                    listMap.append(mapStringMap);
                }

            }
        }

        //如果为false，说明没有读到对应的父元素，应该解析下面的子元素
        if(! isRead)
        {
            //whileReadXml(xml, mapString);
            QStringList  strList = mapString.keys();

            bool aisRead = false;  //是否读到元素，没有读到则跳过当前元素，直接读取下一个元素

            for(auto xmlKey:strList)
            {
                if(xmlKey == strName)
                {
                    aisRead = true;
                    QString xmlElement = xml.readElementText();
                    tempMapString[strName] = xmlElement;
                }
            }
            //跳过结束下面的赋值
            if(!aisRead)
            {
                xml.skipCurrentElement();
            }
        }
    }// end ![0]

    //进行复制品赋值，准备清空listMap
    tempListMap = listMap;
    //清空listMap,每次返回都需要清空
    listMap.clear();
    //情况mapstring的引用，准备重新赋值新的过滤好的数据
    mapString.clear();
    //赋值过滤好的Xml元素属性数据
    mapString = tempMapString;

    //返回复制品的listMap
    return tempListMap;
}

QString HelpClass::xmlErrorString(QXmlStreamReader & xml)
{
    return QString("Error:%1  Line:%2  Column:%3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}

//加载qss文件
QString HelpClass::loaderQSSFile(const QString & qssFileName)
{
    QString openfileName = qssFileName;
    if(!qssFileName.startsWith(":/"))
    {
        openfileName = QString(":/qss/%1.qss").arg(qssFileName);
    }
    QFile file(openfileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical("QSS File '%s' does not open!", qUtf8Printable(qssFileName));
        return "";
    }

    QString styleSheet = QLatin1String(file.readAll());
    file.close();

    return styleSheet;
}

void HelpClass::setGlobalStyleSheet(const QString &qssFileName)
{
    QString qssFile = loaderQSSFile(qssFileName);
    qApp->setStyleSheet(qssFile);

    return;
}
