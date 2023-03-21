#include "filelisten/notify_manage.h"

#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QString>

#include <iostream>

using namespace std;
void setDebugOutput(const QString &rawTargetFilePath_, const bool &argDateFlag_)
{
    static QString rawTargetFilePath;
    static bool argDateFlag;
    rawTargetFilePath = rawTargetFilePath_;
    argDateFlag = argDateFlag_;
    class HelperClass
    {
    public:
        static void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &message_)
        {
            QString message;

            switch (type)
            {
                case QtDebugMsg:
                {
                    message = message_;
                    break;
                }
                case QtWarningMsg:
                {
                    message.append("Warning: ");
                    message.append(message_);
                    break;
                }
                case QtCriticalMsg:
                {
                    message.append("Critical: ");
                    message.append(message_);
                    break;
                }
                case QtFatalMsg:
                {
                    message.append("Fatal: ");
                    message.append(message_);
                    break;
                }
                default: { break; }
            }

            QString currentTargetFilePath;
            currentTargetFilePath = rawTargetFilePath;
            if (!QFileInfo::exists(currentTargetFilePath)) {
                QDir().mkpath(QFileInfo(currentTargetFilePath).path());
            }

            QFile file(currentTargetFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Append);

            QTextStream textStream(&file);
            textStream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << ": " << message << endl;
            cout << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz").toStdString() << ": " << message.toStdString() << endl;
        }
    };
    qInstallMessageHandler(HelperClass::messageHandler);
}
#include <QDebug>
int main(int argc, char* argv[])
{
    setDebugOutput("/home/intify.log", false);
    QCoreApplication a(argc, argv);
    // qRegisterMetaType<FileListener::OperationNode>("FileListener::OperationNode");

    // NotifyManage manager;

    // manager.addRootDir("/home/yang");

    // manager.addUDiskDir("/media/yang/ARCH_202109");

    // manager.startWatch();
    QString fileName = "1.txt";
    QString tmp = fileName;
    tmp.replace(QRegExp("\\(副本 *\\d*\\)$"),"");
    if (tmp == fileName) {
        tmp.replace(QRegExp("\\(副本 *\\d*\\)\\."),".");
    }
    fileName = tmp;
    qDebug() << fileName;
    fileName = "890890(副本).txt";
    tmp = fileName;
    tmp.replace(QRegExp("\\(副本 *\\d*\\)$"),"");
    if (tmp == fileName) {
        tmp.replace(QRegExp("\\(副本 *\\d*\\)\\."),".");
    }
    fileName = tmp;
    qDebug() << fileName;
    fileName = "890890(副本)(副本).txt";
    tmp = fileName;
    tmp.replace(QRegExp("\\(副本 *\\d*\\)$"),"");
    if (tmp == fileName) {
        tmp.replace(QRegExp("\\(副本 *\\d*\\)\\."),".");
    }
    fileName = tmp;
    qDebug() << fileName;
    fileName = "890890(副本)(副本 1)";
    tmp = fileName;
    tmp.replace(QRegExp("\\(副本 *\\d*\\)$"),"");
    if (tmp == fileName) {
        tmp.replace(QRegExp("\\(副本 *\\d*\\)\\."),".");
    }
    fileName = tmp;
    qDebug() << fileName;

    return a.exec();
}