/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     yanghongwei <yanghongwei@uniontech.com>
*
* Maintainer: yanghongwei <yanghongwei@uniontech.com>
*/

#ifndef NOTIFYMANAGE_H
#define NOTIFYMANAGE_H

#include "common.h"

#include <QObject>
#include <QReadWriteLock>

class Fanotify;
class NotifyManage : public QObject
{
    Q_OBJECT
public:
    struct UdiskInfo {
        QString m_uDiskName;
        QString m_vendorID;
        QString m_productID;
        QString m_serailID;
        QString m_devNode;

        bool operator==(const UdiskInfo &b) {
            return ((this->m_uDiskName == b.m_uDiskName) && (this->m_vendorID == b.m_vendorID)
                    && (this->m_productID == b.m_productID) && (this->m_serailID == b.m_serailID)
                    && (this->m_devNode == b.m_devNode));
        }
    };

public:
    explicit NotifyManage(QObject *parent = nullptr);

    void addUDisk(const UdiskInfo &info);
    void removeUDisk(const UdiskInfo &info);

    void addUDiskDir(const QString &path);
    void removeUDiskDir(const QString &path);
    void addRootDir(const QString &path);
    void removeRootDir(const QString &path);
    void startWatch();

    void setUserName(const QString &username) { m_userName = username; }

private:
    void addDir(const QString &path);
    void removeDir(const QString &path);
    void clearOldData(QMap<QString, FileListener::OperationNode> &nodeMap, int timeout);
    QPair<QString, QString> copyUDisk(const QString &path1, const QString &path2);

    void saveHistory(const QString &path, const QString &fileName, qint64 dateTime, const QString &operation);

    void clearUnmountDisk();

private slots:
    void operationHappen(FileListener::OperationNode node);
    void processNode();

Q_SIGNALS:
    void fileChanged();

private:
    Fanotify *m_inotify;

    QMap<QString, FileListener::OperationNode> m_mapWrite;   // 访问操作的所有数据
    QMap<QString, FileListener::OperationNode> m_mapRead;    // 创建文件操作的所有数据

    QStringList m_rootDirs;                                   // 被监听的本地目录
    QStringList m_uDiskDirs;                                  // 被监听的U盘目录

    QReadWriteLock m_rwLock;            // 跨线程操作m_noFindUdiskList数据的锁
    QList<UdiskInfo> m_noFindUdiskList; // 未找到挂载目录的U盘
    QMap<QString, UdiskInfo> m_mapMountUdisk; // 已经找到挂载点的U盘

    QString m_userName;
};

#endif // NOTIFYMANAGE_H
