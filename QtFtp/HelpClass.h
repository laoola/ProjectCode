#ifndef HELPCLASS_H
#define HELPCLASS_H

/**
* @brief: 帮助类
* @author: Fu_Lin
* @date:2018年8月21日
* @description: 主要用于一些读写文件，加密解密，等一些可公共调用的方法
*/

#include <QObject>
#include <QByteArray>
#include <QPoint>
#include <QLabel>
#include "Typedefalias.h"
#include <QLibrary>
#include <QDebug>
#include <QProcess>
#include <QPointer>
#include <QPair>
#include <QFile>
#ifdef Q_OS_WIN32
#include "MemoryModule.h"
#include <string.h>
#endif
#ifdef Q_OS_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#endif

#define DefaultKEY "1edK中.@"  //加密會用到
#define AgencyFileName "agency.dat"

//定义qmlApp宏是为了屏蔽 ToolTip 这个QWidget的弹窗类，说明目前是qml的应用程序
#define QMLAPP

//自定义的对称AES加密密钥，只能用于和某处协议好以后使用
#define AESKEY "X&T/^Re123=~@$yg"

class QJsonObject;
class QJsonDocument;
class QJsonParseError;
class QXmlStreamReader;
class QTranslator;

typedef QMap<QString ,QString>  QMapString;

class HelpClass : public QObject
{
    Q_OBJECT

public:
    explicit HelpClass(QObject *parent = nullptr);

    /**
     * @brief dynameLoaderDllFile 这个是动态加载dll文件路径
     * @param dllFilePath dll文件的路径
     * @param funcPoint dll文件的路径
     * @param funcName dll中方法的名称
     * @note 此方法加载的硬盘的dll文件，必须存在该文件
     * @example 直接给一个dll的所在路径加载
     *  typedef int (*MyPrototype)(int, int); //函数指针
        MyPrototype add; //初始化变量
        //传入路径，函数指针，方法名称，返回对应的函数指针
        add = HelpClass::dynameLoaderDllFilePath(m_dllPath, add, "test4");

       //判断函数指针有效，则进行方法参数调用操作
       if (add)
        {
            qInfo()<<"resule = "<< add(2,6);
        }
     */
    template<typename T2>
    static  inline auto dynameLoaderDllFilePath( const  QString  & dllFilePath,  T2 FuncPoint, const char * FunNameStr)->decltype(FuncPoint){
//#ifdef Q_OS_WIN32


        //路径不存在直接返回空
        if(!QFile::exists(dllFilePath))
        {
            qWarning(".so File not exists !!!");

            return Q_NULLPTR;
        }

        //可以通用，但是调用此方法一定要析构 myLib
        QLibrary * myLib = new QLibrary;


        myLib->setFileName(dllFilePath);
        if (myLib->load())
        {
            myLib->deleteLater();
            return  reinterpret_cast<decltype(FuncPoint)>(myLib->resolve(FunNameStr));
        }
//#endif
//#ifdef Q_OS_LINUX

//        QByteArray dllFile = dllFilePath.toUtf8();
//        const char * charFilePath  = dllFile.data();
//        qInfo()<<"charFilePath"<<charFilePath;
//        qInfo()<<"文件是否存在"<<QFile::exists(dllFile);
//        auto plib = dlopen(charFilePath, RTLD_NOW | RTLD_GLOBAL);
//        if(Q_NULLPTR == plib )
//        {
//            qWarning("Can't open the .so File");
//        }

////        bool ok =  false;//QFile::remove(dllFilePath);
////        if(!ok)
////        {
////            qInfo()<<"删除的路径dllFilePath = "<<dllFilePath;
////            //QProcess::execute(QString("rm -rf  %0").arg(QString(dllFilePath)));
////            qWarning("remove .so file Faile");
////        }

//       return reinterpret_cast<decltype(funcPoint)>(dlsym(plib,  FunNameStr));
//#endif
        return Q_NULLPTR;

    }
    template<typename T2>
     static  inline auto dynameLoaderDllFilePath( const  QByteArray  & dllStream,  T2 FuncPoint, const char * FunNameStr)->decltype (FuncPoint)
     {
         QString tempDir  = getCurrentAppDataDir();
         tempDir +="/.tempFile/";

         HelpClass::isDirExist(tempDir);
         QByteArray dllFilePath;
         dllFilePath = tempDir.toUtf8()+"a.dll";


         QFile files(dllFilePath);
         files.open(QIODevice::WriteOnly);
         files.write(dllStream);
         files.close();

         //const char * charFilePath  = dllFilePath.data();
          hideDirFile(dllFilePath);
         auto dllData =  dynameLoaderDllFilePath(QString(dllFilePath), FuncPoint, FunNameStr);

         if(removeFile(dllFilePath))
         {
             qInfo()<<"delete file finish";
         }
         else
         {
             qInfo()<<"delete file error";
         }
         return dllData;
     }
     /**
      * @brief dynameLoaderDllFilePath 动态加载dll文件路径，由程序生成dll的磁盘路径
      * @param dllStream dll的文件流，并不存在于磁盘上
      * @return dll的动态库指针,第一个参数是生成好的dll文件路径，第二个参数dll的指针
      */
     static  QPair<QString, QLibrary* > dynameLoaderDllFilePath(const  QByteArray  & dllStream, const QString & fileName = "QQIPC.dll",  bool isModifyFileTimer = false)
     {
         QPair<QString, QLibrary* >  pairResult;
         QString tempDir  = getCurrentTempDataDir();
         tempDir +="/.QQMasters/";

         HelpClass::isDirExist(tempDir);
         QString dllFilePath;
         dllFilePath = tempDir.toUtf8()+ fileName;
         QString suffix = "dll";

#ifdef Q_OS_ANDROID
         suffix = "so";
#endif
        checkFileSuffix(dllFilePath, suffix);
         QFile files(dllFilePath);
         if(!files.open(QIODevice::WriteOnly))
         {
             qWarning()<<"dynameLoaderDllFilePath file --"<<dllFilePath+" Open Fail";
             pairResult.first = "-1";
             return pairResult;
         }
         files.write(dllStream);
         files.close();
         pairResult.first = dllFilePath;
         pairResult.second = dynameLoaderDllFilePaths(dllFilePath,isModifyFileTimer);
         return pairResult;
     }
     /**
      * @brief dynameLoaderDllFilePath 动态加载dll文件路径，直接给文件路径
      * @param dllFilePath dll的文件路径，已经存在磁盘的dll路径
      * @return 返回dll的加载指针
      */
     static  QLibrary * dynameLoaderDllFilePaths(const  QString  & dllFilePath, bool isModifyFileTimer= false)
     {
         //路径不存在直接返回空
         if(!QFile::exists(dllFilePath))
         {
             qWarning(".so or .dll File not exists !!!");

             return Q_NULLPTR;
         }


         if(isModifyFileTimer)
         {
             hideDirFile(dllFilePath);
             //修改文件的生成时间
             modifyFileTime(dllFilePath, "2018-09-04", "20:28:50");
         }


         //!可以通用，但是调用此方法一定要外面注意析构 myLib
        QLibrary * myLib = new QLibrary;

         myLib->setFileName(dllFilePath);
         if (myLib->load())
         {
             return  myLib;
         }

         return Q_NULLPTR;
     }
    /**
     * @brief dynameLoaderDllFile 这个是动态加载dll文件路径 （重载函数）
     * @param dllFile dll文件的路径
     * @param funcPoint
     * @note 此方法是从内存加载dll，可以通过网络或者读取dll文件获取流到内存
     * @example 从文件读取一个dll，然后加载
     *  QFile file(m_dllPath);

        if(!file.open(QIODevice::ReadOnly))
        {
              qFatal("打开文件失败!!!");
        }
        char * dllFileData;

         //读取到所有的dll内容流
        QByteArray dllFileByte= file.readAll();

        //转换成char*内容
        dllFileData = dllFileByte.data();
        typedef int(*AddFunc)(int, int); // 定义函数指针类型
        //初始化函数指针
        AddFunc add;

        //调用从内存加载方法进行dll处理
        add = HelpClass::dynameLoaderDllStream(dllFileByte, add,"test4");

        //判断返回的函数指针，如果有效则进行参数调用
         if (add)
         {
             qInfo()<<"resule = "<< add(1,2);
         }
    */
    template<typename T2>
    static  inline auto dynameLoaderDllStream(const QByteArray &dllStreamFile,  T2 FuncPoint, const char * FunNameStr)->decltype(FuncPoint)
    {
#ifdef Q_OS_WIN32

        HMEMORYMODULE hm = MemoryLoadLibrary(dllStreamFile);

        return reinterpret_cast<decltype(FuncPoint)>(MemoryGetProcAddress(hm, FunNameStr));

#endif
#ifdef Q_OS_LINUX
        QString tempDir  = getCurrentTempDataDir();
        tempDir +="/.tempFile/";

        HelpClass::isDirExist(tempDir);
        QByteArray dllFilePath;
        dllFilePath = tempDir.toUtf8()+".aa.so";

        QFile files(dllFilePath);
        files.open(QIODevice::WriteOnly);
        files.write(dllStreamFile);
        files.close();

        const char * charFilePath  = dllFilePath.data();

        auto plib = dlopen(charFilePath, RTLD_NOW | RTLD_GLOBAL);
        //加载以后立马将文件移除
        bool ok= removeFile(dllFilePath);
        if(!ok)
        {
            qWarning("remove .so file Faile");
        }

        if(Q_NULLPTR == plib )
        {
            qWarning("Can't open the .so File");
            return nullptr;
        }
       return reinterpret_cast<decltype(FuncPoint)>(dlsym(plib,  FunNameStr));

#endif
    }
    /**
     * @brief onlyOpenOnceEXE 调用此方法，exe程序只能打开一次，再次打开会有提示
     */
    static void onlyOpenOnceEXE();

    /**
     * 生成 [ 0 - nMax ]范围内不重复的数据 nCount 个
     * 注意， nMax 不小于 nCount
     * trueRandom = true 真随机(每次开机启动随机数都不一样)，=false假随机(每次开机启动随机数都一样)
     *
     */
    static QList<int> random(int nMax, int nCount, bool trueRandom = true);

    //替换特殊字符，\r, \n \t等
    static void replaceCharacters(QString & chara);
    /**
     * @brief aes128Encryption 128位的AES 加密
     * @param key 需要加密的key
     * @param plaintext 加密的明文
     * @return  返回加密
     */
    static QByteArray aes128Encryption(const QByteArray & key, const QByteArray & plaintext);
    static QByteArray aes128Decrypt(const QByteArray & key, const QByteArray & encodedText);

    /**
     * @brief languageSettings 自动设置中英文语言环境
     * @param languageQmFile 翻译好的qm文件,用list是方便后面可能需要跟多国家的语言，这样后续扩展更方便，
     *                                       也不会干扰到前面调用过该方法的代码，但是顺序需要外面配合好，具体顺序标记看下面说明
     *
     * @param autoLoader 自动识别系统语言加载对应的翻译文件, true 就是自动，false就是手动，手动参考loaderType值
     * @param loaderType 当autoLoader = false时会根据其值来选择加载哪一个语言文件，0 --英语，1--中文
     * @return  返回当前的环境，0代表英文，1代表中文后续根据情况再添加，目前只有中文和英文
     */
    static int languageSettings(const QStringList & languageQmFile , bool  autoLoader = true, int loaderType = 0);


    static QByteArray toHexBase64(QByteArray data);

    static QByteArray toHexBase64(QString data);
    /**
     * @brief modifyFileTime 修改文件的生成，创建等时间信息
     * @param filePathName 文件的路径名称，包含文件名称，如（C:\Windows\System32\sv-SE\WWAHost.exe.mui）
     * @param date 需要更改的日期，如2018-09-04
     * @param time 需要更改的时间，如20:28:50
     * @note data和time必须按照上面的格式来赋值，并且要分开
     */
    static void modifyFileTime(const QString & filePathName, const QByteArray & date, const QByteArray &  time);

    /**
     * @brief GetEncrypt 获取加密后的密文
     * @param plaintextStr 加密前的明文 QByteArray类型
     * @param key 加密需要的钥匙，解密也要用到，建议不填写，使用默认自带钥匙
     * @return 已加密成功的密文
     */
    static QByteArray GetEncrypt(const QByteArray plaintextStr, QByteArray key ="");

    /**
      * @brief GetEncrypt 获取加密后的密文--重载函数
      * @param plaintextStr 加密前的明文 QString类型
      * @param key  加密需要的钥匙，解密也要用到，建议不填写，使用默认自带钥匙
      * @return 已加密成功的密文
      */
    static QByteArray GetEncrypt(const QString plaintextStr, QByteArray key ="");

    /**
     * @brief GetDecrypt 获取解密后的明文
     * @param ciphertext 解密前的密文， 必须为QByteArray类型
     * @param key 加密需要的钥匙，解密也要用到，建议不填写，使用默认自带钥匙
     * @return 已解密的明文
     */
    static QByteArray GetDecrypt(const QByteArray ciphertext, QByteArray key ="");


    /**
     * @brief hideDirFile 隐藏文件夹或文件
     * @param dirPath 给出需要隐藏的文件或文件夹
     */
    static void hideDirFile(QString dirPath);

    /**
     * @brief showDirFile 显示被隐藏的系统文件
     * @param dirPath
     */
    static void showDirFile(QString dirPath);
    /**
     * @brief isDirExist 判断文件夹是否存在,不存在则创建
     * @param fullPath 需要判断并创建的路径，支持多级子路径创建
     * @return 成功返回true, 失败返回false
     */
    static bool isDirExist(QString fullPath);

    //字符串是否包换数字
    static bool isStrContainsNumbers(const QString & str);
    /**
     * @brief fileDataWrite 将数据写入到给定的文件路径(包括文件名)
     * @param filePath 文件的路径(包括文件名称)
     * @param fileData 文件的数据
     */
    static void fileDataWrite(const QString &filePath, const QByteArray & fileData);
    /**
     * @brief checkFileSuffix 检查一个给点字符串和对应的文件后缀，如果有则不追加周追，否则追加后缀在后面
     * @param filePath 文件的路径(必须包括文件名)
     * @param suffix 文件的后缀
     */
    static void checkFileSuffix(QString & filePath, const QString &suffix);

    /**
     * @brief bytesToKB  字节 转换为B KB MB GB
     * @param bytes 需要转换的字节数
     * @return 转换成功的对应KB, 大小，默认保留两位小数
     */
    static QString bytesToKB(qint64 size);

    /**
     * @brief writeDataToFile 将数据写入到文件，数据格式采用QList<QMap>类型
     * @param mapList 数据的map类型，直接将map数据写入文件，取出来的时候也是对应的时候也是对应map类型
     * @param Path 写入文件的路径名称，建议为空，使用默认参数
     * @return true 写入成功，false 写入失败
     */
    static bool writeDataToFiles(QList<QMap<QString , QStringList> > mapList, QString Path = "");
    static bool writeDataToFile(QList<QMap<QString , QString> > mapList, QString Path = "");

    /**
     * @brief ReadDataFromFile 读取文件
     * @return  返回List<Map>类型
     */
    static QList<QMap<QString , QStringList> > ReadDataFromFiles(QString Path = "");
    static QList<QMap<QString , QString> > ReadDataFromFile(QString Path = "");
    /**
     * @brief removeFile 删除传入的文件
     * @param filePath 文件的绝对路径
     * @return
     */
    static bool removeFile(QString filePath="");
    /**
     * @brief removeDirFile 删除指定目录的所有文件
     * @param dirPath 目录路径,如果为空则删除临时目录下的指定一个文件夹
     * @return true删除成功，false删除失败
     */
    static bool removeDirFile(const QString & dirPath = "");
    /**
     * @brief deleteFile 强制删除被占用的文件
     * @param filePath 文件的路径
     */
    static void forcedDeleteFile(QString filePath);

    /**
     * @brief getTimestamp 获取当前时间戳
     * @return  QString类型时间戳
     */
    static QString getTimestamp();

    /**
     * @brief md5Encrypt 进行md5加密
     * @param plaintextStr 需要加密的明文
     * @return
     */
    static QByteArray md5Encrypt(QString plaintextStr);

    static QByteArray toHexMd5Encrypt(const QByteArray &plaintextStr);
    /**
     * @brief getCurrentTime 获取当前时间
     * @return
     */
    static QString getCurrentTime(const QString &format = "");

    /**
     * @brief getCurrentDateTime 获取当前的年月日
     * @param format 转换格式
     * @return
     */
    static QString getCurrentDateTime(const QString &format = "");

    /**
     * @brief msecsTo 转换毫秒数，获取从传送过来的时间到当前时间之差
     * @param timeStr 传送过来的时间
     * @return 时间差，毫秒级时间戳
     */
    static QString msecsTo(const QString &timeStr);

    /**
     * @brief jsonCreate jsonObject 对象的生成
     * @param key  需要生成的key
     * @param value 需要生成的 value
     * @param isClean 是否生成前先清空jsonObject
     */
    static void jsonCreate(const QString &key, const QJsonValue &value, bool isClean=false);

    /**
    * @brief getJsonObject 调用此方法之前，需要先调用jsonCreate，来生成jsonObject
    * @return
    */
    static QJsonObject getJsonObject();

    /**
    * @brief tojsonArray 将json数组转换为字符串格式
    * @return
    */
    static QString tojsonArray(QJsonArray);

    /**
    * @brief generalJsonParse 通用的json解析方法 ,只能支持一级json解析，数组型的暂不支持
    * @param jsonDocument   需要解析的QJsonDocument,由外面调用传入
    * @param jsonkeyMap     需要解析的json中的Key值，首次传入，value应该为空，调用完成后，再根据key取出对应的value
    * @param headKey        json需要解析的头字段key，默认为code，如果code值不对，直接返回false，Map不做操作，code值请参考headValue
    * @param headValue      json需要解析的头字段key匹配的正确json返回值， 默认正确为0
    * @param flage          跳过json headKey和value的对比，直接截取对应需要的字段
    * @return true 解析成功，false 解析失败
    */
    static bool generalJsonParse(QJsonDocument, QMap<QString ,QString> &jsonkeyMap, QString headKey = "code", int headValue = 0, bool flage = false);

    /**
    * @brief generalarrayJsonParse  通用的json数组解析方法 ,只能支持一级数组json解析，数组嵌套型的暂不支持
    * @param jsonDocument 需要解析的json,QJsonDocument,由外面调用传入
    * @param listJsonkepMap 需要保存的json数组中的Key值，调用完成后，再根据key取出对应的value
    * @param arrayKey 需要解析判断的json数组中的key值，如果判断不是数组类型，直接返回false，
    * @param headKey  json需要解析的头字段key，默认为code，如果code值不对，直接返回false，Map不做操作，code值请参考headValue
    * @param headValue  json需要解析的头字段key匹配的正确json返回值， 默认正确为0
    * @param flage 如果为true跳过headKey的匹配检查
    * @return 解析成功为true， 解析失败返回false
    */
    static bool generalArrayJsonParse(QJsonDocument jsonDocument, QList<QMapString> & listJsonkepMap, QMapString jsonkey, QString arrayKey, QString headKey = "code", int headValue = 0, bool flage = false);

    /**
     * @brief parseJsonData 解析json数据，可以解析一般的数组和非数组数据，嵌套太深自己看着办，一般json结构都能满足
     * @param json json数据
     * @param jsonData 最终的jsonData，QVariant如果是数组则是QVariantList,非数组里面则是QVariant数据，具体内部自行判断
     * @param rootkey 所有需要解析的jsonkey，包含数组根节点
     */
    static void parseJsonData(QByteArray json, QMap<QString, QVariant> &jsonData, QStringList rootkey);
    //上面的函数重载
    static void parseJsonData(QJsonDocument json, QMap<QString, QVariant> &jsonData, QStringList rootkey);
    //下面两个私有方法只允许parseJsonData调用，外面不能调用
private:
    static void parseJsonObject(QJsonObject jsonObject, QMap<QString, QVariant>  & jsonData,QStringList parseKey) ;
    static void parseJsonArray(QJsonArray jsonArray, QMap<QString, QVariant>  & jsonData, QStringList parseKey, QString arraykey) ;

public:
    /**
     * @brief jsonAllFormatsParse json万能格式解析，所有嵌套的格式都能解析
     * @param jsonDocument 需要解析的json文档
     * @param parseJsonKey 需要解析的jsonKey, 包含了所有要解析的key，根节点key也要包含
     * @param rootKey 所有的根节点key，虽然和上面重复，但是有其用处,如果不是数组的key，只允许出现一次，看例子说明
     * @return 返回的是一个QMap类型，每个根节点key 对应旗下的所有子节点数据，如果是数组型根节点，后面会自动补数字，
     *               例如a[{1}, {2}, {3}].那么QMap返回的key就是a0,a1,a2
     * QPair<QMap<QString, int> 每个解析节点的个数
     * QMap<QString,  QList<QMapString>>> 每个数组解析节点后面追加数字和对应的list数据
     * @example
     *  下面是一段复杂的嵌套 json：
          {
            "data": "你好吗？",  //根节点
            "name":"我也是根节点", //根节点，--data和name只能取一个为根节点，不然会有冲突，也就是root下的不是数组的根节点，全部只能多取1，取其中一个
            "dataArray": [ //根节点
                {
                    "initA": "A1",
                    "initB": [
                        {
                            "subA": "AA1",
                            "subB": [
                                {
                                    "subAA": "AAA1",
                                    "subBB": "BBB1"
                                }
                            ]
                        },
                        {
                            "subA": "AA11",
                            "subB": "BB11"
                        }
                    ],
                    "initC": "C1"
                },
                {
                    "initA": "A2",
                    "initB": [
                        {
                            "subA": "AA2",
                            "subB": "BB2"
                        },
                        {
                            "subA": "AA22",
                            "subB": "BB22"
                        }
                    ],
                    "initC": "C2"
                }
            ],
            "testArray": [  //根节点
                {
                    "initA": "test1",
                    "initB": "testB"
                },
                {
                    "initA": "test1A",
                    "initB": "testBA"
                }
            ]
        }

    //需要解析的key，包含根节点，一定要包含
    QStringList jsonparseKey = {"data","dataArray","testArray", "initA","initB","initC","subA","subB", "subAA","subBB"};
    //所有的根节点
    QStringList rootKey = {"data", "dataArray", "testArray"};
    QMap<QString,  QList<QMapString>> mapListStr= jsonAllFormatsParse(jsonDocum, jsonparseKey, rootKey);
    返回数据如下：
    QMap(("data", (QMap(("data", "你好吗？"))))
              ("dataArray0", (QMap(("subAA", "AAA1")("subBB", "BBB1")), QMap(("subA", "AA1")), QMap(("subA", "AA11")("subB", "BB11")), QMap(("initA", "A1")("initC", "C1"))))
              ("dataArray1", (QMap(("subA", "AA2")("subB", "BB2")), QMap(("subA", "AA22")("subB", "BB22")), QMap(("initA", "A2")("initC", "C2"))))
              ("testArray0", (QMap(("initA", "test1")("initB", "testB"))))("testArray1", (QMap(("initA", "test1A")("initB", "testBA")))))
     自行对应上面的复杂json看
     *
     */
    static QPair<QMap<QString, int>, QMap<QString,  QList<QMapString>>> jsonAllFormatsParsePair(QJsonDocument jsonDocument, QStringList parseJsonKey, QStringList rootKey);


    /**
     * @brief jsonAllFormatsParseMap 这个方法的返回值比较轻快，不需要用户二次解析，主要填入对应的key，
     *                                                  取出一串stringlist即可，但是层次感不强，需要有层次感结构需求取得，请参考jsonAllFormatsParseMap_II方法
     * @param jsonDocument 同上或见下
     * @param parseJsonKey 同上或见下
     * @param rootKey 同上或见下
     * @return //!返回每个解析节点关键词对应的数据，如上面的例子中，参数说明同上
        //! ("dataArray0", (QMap(("subAA", "AAA1")("subBB", "BBB1")), QMap(("subA", "AA1"))
        //! ("dataArray1", (QMap(("subA", "AA2")("subB", "BB2"))
        //! 会整合返回成：subAA: [AAA1],subA:[AA1, AA2] subBB:[BBB1],subB:[BB2]相当于关键词集合,[]代表stringList
     */
    static QMap<QString,QStringList>jsonAllFormatsParseMap(QJsonDocument jsonDocument, QStringList parseJsonKey, QStringList rootKey);

    /**
     * @brief jsonAllFormatsParseMap_II 这个方法的返回值比较具有层次感，可能需要用户进行二次解析，
     *                                                      但是基本是for循环判断取出来即可, 因为比较复杂，下面我会给出详细的例子进行说明
     * @param jsonDocument json转换的文档
     * @param parseJsonKey 需要解析的key，包含根节点，所有的需要解析的key都要填上
     * @param rootKey 不是数组的根节点，只能多取一
     * @return   //!返回值是每个大的节点对应，小的节点数据，如上面例子中, 参数说明同上
                    //! ("dataArray0", (QMap(("subAA", "AAA1")("subBB", "BBB1")), QMap(("subA", "AA1"))
                    //! ("dataArray1", (QMap(("subA", "AA2")("subB", "BB2"))
                    //! 会整合返回成：dataArray:[QMap(("subAA", "AAA1")("subBB", "BBB1")), QMap(("subA", "AA1"),QMap(("subA", "AA2")("subB", "BB2")]
                    //! dataArray会去掉所有的数字，集合对应相同的key的数据，属于大关键词结合数据
                    //! 建议使用此方法可能更精确一点，说明一点，这里如果不是数组，所有的单个变量请求的key都是root
                    //! 这个方法的返回值比较绕，看下嘛例子
        @example 下面是一个比较复杂的json例子，嵌套三层，打印结果可以说明，嵌套N层都是可以的，层次感很明显
                {
                    "name": "小明",
                    "age": 12,
                    "six": "男",
                    "result": [{
                            "y": "A",
                            "s": "A+",
                            "yy": "B",
                            "dl": [{
                                "dlA": "C",
                                "dlB": "B",
                                "dlC": [{
                                    "A": "B",
                                    "B": "C+"
                                }, {
                                    "A": "D",
                                    "B": "e+"
                                }],
                                "dlD": "D"
                            }, {
                                "dlA": "A",
                                "dlB": "B+",
                                "dlC": [],
                                "dlD": "C-"
                            }]
                        },
                        {
                            "y": "A-",
                            "s": "B+",
                            "yy": "C",
                            "dl": []
                        },
                        {
                            "y": "B-",
                            "s": "D+",
                            "yy": "A",
                            "dl": [{
                                "dlA": "CC",
                                "dlB": "BB-",
                                "dlC": [],
                                "dlD": "D"
                            }]
                        }

                    ]
                }
   //需要解析的key和相应的根节点key，都要填上去
   QStringList jsonparseKey = {"name","age","six", "result","y","s","yy","dl", "dlA","dlB","dlC", "A", "B"};

   ///这里必须根节点多取一(也就是非数组节点的，要多个里面选一个就好了，都会取到的)，
   /// 本来应该是name， age，six都是根节点，但是他们都不是数组节点，所以要多取一，我取第一个
   QStringList rootKey = { "name" ,"result"};

   填入参数，调用方法即可
   QMap<QString , QList<QList<QMapString>>> mapString = HelpClass::jsonAllFormatsParseMap_II(jsonDocum,jsonparseKey,rootKey);

   返回值打印：
   QStringList keys = mapString.keys();
    for(auto key:keys)
    {
        qInfo()<<"key = "<<key;
        qInfo()<<"size = "<<mapString[key].length();
        int i = 0;
        for(auto mapValue:mapString[key])
        {
            qInfo()<<"i =" <<i;
            qInfo()<<"mapValue = "<<mapValue;
            i++;
        }
    }

    打印结果如下：(可以看出是返回的根节点result是很具有层次的，自己对着上面的json来看)
        key =  "Root"
        size =  1
        i = 0
        mapValue =  (QMap(("age", "12")("name", "小明")("six", "男")))
        key =  "result"
        size =  3
        i = 0
        mapValue =  (QMap(("A", "B")("B", "C+")), QMap(("A", "D")("B", "e+")), QMap(("dlA", "C")("dlB", "B")), QMap(("dlA", "A")("dlB", "B+")), QMap(("s", "A+")("y", "A")("yy", "B")))
        i = 1
        mapValue =  (QMap(("s", "B+")("y", "A-")("yy", "C")))
        i = 2
        mapValue =  (QMap(("dlA", "CC")("dlB", "BB-")), QMap(("s", "D+")("y", "B-")("yy", "A")))
     */
    static QMap<QString, QList<QList<QMapString>> > jsonAllFormatsParseMap_II(QJsonDocument jsonDocument, QStringList parseJsonKey, QStringList rootKey);

    /**
    * @brief getUuid 获取Uuid唯一标识符 ，经测试不会重复出现
    * @return
    */
    static QString getUuid();

    /**
    * @brief getCurrentTempDataDir 获取app数据路径
    * @return 类似于这样: "C:/Users/11427/AppData/Local/STC/pcVehicleSystem"
    */
    static QString getCurrentAppDataDir();

    static QString getCurrentTempDataDir();

    static QString getCurrentFontDir();
    /**
    * @brief loaderQssFile 加载QSS样式表文件
    * @param qssFileName  根据传送过来的qss文件名称加载不同的路径文件（可以传全路径或者当前的文件名）
    * @return 返回已经读取成功的qqs文件数据，返回空代表打开文件失败
    */
    static QString loaderQSSFile(const QString &qssFileName);

    /**
    * @brief setGlobalStyleSheet 设置全局样式表格式
    * @param qssFileName 需要设置的样式表文件
    * @return 无
    */
    static void setGlobalStyleSheet(const QString &qssFileName);

    /**
    * @brief writeSettingFile 将相关数据写入配置文件
    * @param mapSettingData 需要写入的map数据
    * @param groupName  写入setting自定义的组名，读取的时候也要对应的组名
    * @param filePath 需要写入的文件路径（包含文件名称）
    * @param isEncrypt 写入时时候加密，缺省值为不加密
    */
    static void writeSettingFile(QMap<QString , QString> mapSettingData, QString groupName, QString filePath, bool isEncrypt = false);

    /**
    * @brief readSettingFile 读取相关配置数据信息
    * @param groupName 需要读取的setting自定义的组名，与写入时的对应
    * @param filePath 需要读取的文件路径(包含文件名称)
    * @param isDecrypt 读取时是否解密,与写入时时候加密对应
    * @return 返回对应的组名的map数据，根据自己定义的key来取值
    */
    static QMap<QString , QString> readSettingFile(QString groupName, QString filePath , bool isDecrypt = false);

#ifndef QMLAPP
    /**
    * @brief ToolTipmessage 消息提示框弹出，默认从最上中间弹出
    * @param parent  弹窗的父对象
    * @param message 弹出消息的显示文本
    * @param point 弹出消息的位置，默认不写，位置在最上中间动画弹出, 如果填写具体位置则无动画弹出
    */
    static void ToolTipmessage(QWidget * parent, QString message, QPoint point = QPoint(0, 0));


    /**
    * @brief getFontWidth 获取字体长度
    * @param text 字体文本
    * @return 字体长度
    */
    static int getFontWidth(QString text);

    /**
    * @brief showDialogText  首先子对象的提示性文本框，如：该数据获取为空等等提示性
    * @param parent 需要显示的父对象
    * @param showText 需要显示提示的文本内同
    * @param free 是否释放该控件文本
    */
    static void showDialogText(QWidget *parent, QString showText, bool free = false);

    /**
    * @brief setGraphicsEffect 设置文本阴影
    * @param parent 需要设置的父对象
    */
    static void setGraphicsEffect(QWidget * parent);
#endif
    /**
    * @brief CopyText  剪切板实现“复制”“粘贴”功能
    * @param text 需要复制的文本
    */
    static void copyText(QString text);

    // 字节转KB、MB、GB(前面好像有一个重复的方法)
    static QString size(qint64 bytes);

    // 速度转KB/S、MB/S、GB/S
    static QString speed(double speed);

    // 秒转*d *h *m *s
    static QString timeFormat(qint64 seconds);


    /**
    * @brief xmlReadParse 读取解析xml流数据（只支持一级格式解析，必须是声明好的key和value对应）
    * @param xmlMapPara 需要解析的xml的参数
    * @param rootElement 需要判断的根元素，如果根元素不对，直接返回false
    * @param xmlDataStream xml的流数据
    * @note 如果发生解析错误，会在xmlMapPara中自动写入key = XMLError,value = 错误的原因，xml哪行错误
    */
    static TypedefAlias::QListMapStringMap xmlReadParse(TypedefAlias::QMapStringMap &xmlMapPara,const QString & rootElement, const QByteArray & xmlDataStream);

    /**
    * @brief xmlReadParse 读取解析xml文件（重载函数， 只支持一级格式解析，必须是声明好的key和value对应）
    * @param xmlMapPara 需要解析的xml的参数，后续会返回对应的参数的值
    * @param rootElement 需要判断的根元素，如果根元素不对，直接返回false
    * @param xmlFile xml文件，需要打开的文件
    * @return 解析好的list对应的map
    */
    static TypedefAlias::QListMapStringMap xmlReadParse(TypedefAlias::QMapStringMap &xmlMapPara, const QString & rootElement, const QString & xmlFile);


    /**
    * @brief xmlReadParse 读取解析xml文件(重载函数， 支持无限格式解析)
    * @param xmlMapPara 需要解析的父元素的子元素的 key和value的值，key必须，value可以为空，也可以随意赋值
    * @param parseElement 需要解析的父元素素，所有根元素的父元素 ，stringList形式
    * @param rootElement 解析判断的xml的根元素，必须指定，否则返回一个空的listMap变量
    * @param xmlDataStream xml的源码数据流
    * @note 可以解析万用xml嵌入格式，递归调用解析，如果解析出错会返回一个ERROR的节点key，并输出一个包裹key为XMLError的Map
    * @example
    * 这是一个xml文件
    * <?xml version="1.0" encoding="ISO-8859-1"?>
        <!-- Edited by XMLSpy® -->
        <CATALOG> //根元素
            <PLANT>  //父元素
                <COMMON>Bloodroot</COMMON>
                <BOTANICAL>Sanguinaria canadensis</BOTANICAL>
                <ZONE>4</ZONE>
                <CD>  //父元素
                    <TITLE>Hide your heart</TITLE>
                    <ARTIST>Bonnie Tyler</ARTIST>
                    <COUNTRY>UK</COUNTRY>
                    <COMPANY>CBS Records</COMPANY>
                    <PRICE>9.90</PRICE>
                    <YEAR>1988</YEAR>
                </CD>
            </PLANT>
        </CATALOG> //根元素结束
        //先赋值需要解析的父元素下的子元素的key和value
        TypedefAlias::QMapString xmlMapPara；
        //下面是PLANT父元素的子元素标签
        xmlMapPara["COMMON"] = "";
        xmlMapPara["BOTANICAL"] = "";
        xmlMapPara["ZONE"] = "";
        //下面是CD父元素的子元素标签，以此类推赋值即可
        xmlMapPara["TITLE"] = "";
        xmlMapPara["ARTIST"] = "";
        xmlMapPara["COUNTRY"] = "";
        //然后赋值需要解析的根下面的需要解析的所有父元素 ,根据上面的xml文件只有PLANT和CD
        QStringList  parseElement = {"PLANT", "CD"};
        //最后调用xmlReadParse，填入对应的参数
       TypedefAlias::QListMapStringMap listMap =  xmlReadParse(xmlMapPara, parseElement, "CATALOG", "填入上面的xml文件流或者对应的xml文件即可");
    * @return listMap =  QMap(("PLANT", QMap(("BOTANICAL", "Caltha palustris")("COMMON", "Marsh Marigold")("ZONE", "4"))))一个list，太长，就写一段
    */
    static TypedefAlias::QListMapStringMap xmlReadParse(TypedefAlias::QMapString &xmlMapPara, const QStringList & parseElement, const QString & rootElement, const QByteArray & xmlDataStream, const QStringList &parseAttributes ={""});
    static TypedefAlias::QListHashStringMap xmlReadParse(TypedefAlias::QHashString &xmlMapPara, const QStringList & parseElement, const QString & rootElement, const QByteArray & xmlDataStream);
    /**
    * @brief xmlReadParse 读取解析xml文件(重载函数， 支持无限格式解析)
    * @param xmlMapPara 需要解析的父元素的子元素的key和value的值，key必须，value可以为空，也可以随意赋值
    * @param parseElement 需要解析的父元素，所有根元素的父元素 ，stringList形式
    * @param rootElement 解析判断的xml的根元素，必须指定，否则返回一个空的listMap变量
    * @param xmlFile xml的文件
    * @note 可以解析所有嵌套的xml嵌入格式，递归调用解析, 如果解析出错会返回一个ERROR的节点key，并输出一个包裹key为XMLError的Map
    * @return 返回已经解析后的xml元素和对应的属性
    */
    static TypedefAlias::QListMapStringMap xmlReadParse(TypedefAlias::QMapString &xmlMapPara, const QStringList & parseElement, const QString & rootElement, const QString & xmlFile);
    static TypedefAlias::QListHashStringMap xmlReadParse(TypedefAlias::QHashString &xmlMapPara, const QStringList & parseElement, const QString & rootElement, const QString & xmlFile);
    //-----------------------------------------结束-----------------------------------------------------------------//


private:
    //读取父元素(和下面的读取子元素配合，只能读取一级操作)
    static TypedefAlias::QListMapStringMap whileReadXml(QXmlStreamReader &xml, TypedefAlias::QMapStringMap &mapStringMap);
    //读取子元素
    static void whileReadXml(QXmlStreamReader &xml, TypedefAlias::QMapString & mapString);

    //循环读取所有的父子元素
    static TypedefAlias::QListMapStringMap whileReadXml(QXmlStreamReader &xml, const QStringList &stringElement, QMapString & mapString, const QStringList &parseAttributes = {""});
    static TypedefAlias::QListHashStringMap whileReadXml(QXmlStreamReader &xml, const QStringList &stringElement, TypedefAlias::QHashString & mapString);

    static QString xmlErrorString(QXmlStreamReader &xml);

    /**
     * @brief encryption base64加密中英文字符
     * @param plaintextStr 加密明文 QByteArray类型
     * @return
     */
    static QByteArray encryption(const QByteArray plaintextStr);
    /**
     * @brief encryption base64加密中英文字符重载函数
     * @param plaintextStr 加密明文，QString类型
     * @return
     */
    static QByteArray encryption(const QString plaintextStr);
    /**
     * @brief Deciphering 解密字符
     * @param ciphertext 需要解密的密文，必须为QByteArray
     * @return
     */
    static QByteArray Deciphering(const QByteArray ciphertext);
    /**
      * @brief getXorEncryptDecrypt 异或加解密方法
      * @param str  需要加密的明文或密文
      * @param key  需要加密的钥匙
      * @return
      */
    static QByteArray getXorEncryptDecrypt(const QByteArray &str, const QByteArray &key);

    /**
     * @brief midDigits 分隔数字出来，删除字符串后面尾巴的数字
     * @param src 带有数字的字符串，目前只用于json万能解析调用
     */
    static void midDigits(QString &src);

    /**
     * @brief parseJsonObject 解析json对象，可用于递归调用
     * @param jsonObject  josn对象
     * @param mapSubKey 需要解析的json子key，其实就是全部的key，包括了根节点
     * @param keyrootList 根节点需要判断的key
     * @param keyroot 当前解析的根key
     * @return 一个链表类型的QMap类型
     * @node 由json万能格式解析调用，详细见jsonAllFormatsParse 方法调用
     */
    static QList<TypedefAlias::QMapStringMap> parseJsonObject(QJsonObject jsonObject, QStringList mapSubKey, QStringList keyrootList, QString keyroot = "root");

    /**
     * @brief parseJsonArray json数组调用，递归调用
     * @param jsonArray json数组
     * @param key 当前需要解析的根节点
     * @param mapSubKey 所有需要解析的json key， 包括了根节点
     * @param keyrootList 需要解析的根节点
     * @return 一个链表类型的QMap类型
     * @note 由parseJsonObject调用
     */
    static QList<TypedefAlias::QMapStringMap> parseJsonArray(QJsonArray jsonArray, QString key, QStringList mapSubKey, QStringList keyrootList);

private:
    static QJsonObject m_jsonObject;
    static QLabel *m_label;
    static QTranslator  m_translator;
};

#endif // HELPCLASS_H
