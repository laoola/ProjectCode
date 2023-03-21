/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     yanghongwei <yanghongwei@uniontech.com>
*
* Maintainer: yanghongwei <yanghongwei@uniontech.com>
*/

#include "notify_manage.h"
#include "fanotify_file_listener.h"

#include <QDebug>
#include <QTimer>
#include <QFileInfo>
#include <QDir>

#include <sys/inotify.h>
#include <mntent.h>

#define TIMEOUT_INTERVAL        500     // 500ms进行一次操作逻辑处理，以及操作500S的操作直接清理掉
#define OPERATION_USB_TO_PC     "pc"
#define OPERATION_PC_TO_USB     "usb"

/**
 * @brief 获取U盘挂载的目录
 *
 * @param devNode 挂载设备
 * @param mountPath 输出的挂载目录
 * @return true
 * @return false
 */
bool getMountDir(const char *devNode, QString &mountPath)
{
    struct mntent *ent;
    FILE *f;
    bool result = false;

    f = setmntent("/etc/mtab", "r");
    if (!f) {
        perror("setmntent");
        exit(1);
    }

    while ((ent = getmntent(f))) {
        if (strcmp(ent->mnt_fsname, devNode) == 0) {
            mountPath = QString(ent->mnt_dir);
            result = true;
            break;
        }
    }
    endmntent(f);
    return result;
}

// 处理复制文件使用共存模式时，复制文件添加（复制）字段，英文环境下为(copy)
void removeFileCopyInfo(QString &fileName)
{
    fileName.remove(QRegExp("\\(副本 *\\d*\\)"));
    fileName.remove(QRegExp("\\(copy *\\d*\\)"));
}

NotifyManage::NotifyManage(QObject *parent)
    : QObject(parent)
    , m_inotify(new Fanotify(this))
{
    connect(m_inotify, &Fanotify::operationHappen, this, &NotifyManage::operationHappen);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NotifyManage::processNode);
    timer->start(TIMEOUT_INTERVAL);
}

void NotifyManage::addUDisk(const UdiskInfo &info)
{
    qDebug() << "yang33333333333";
    m_rwLock.lockForWrite();
    m_noFindUdiskList.append(info);
    m_rwLock.unlock();
}

void NotifyManage::removeUDisk(const UdiskInfo &info)
{
    m_rwLock.lockForWrite();
    m_noFindUdiskList.removeOne(info);
    m_rwLock.unlock();
    for (auto item = m_mapMountUdisk.begin(); item != m_mapMountUdisk.end(); item++) {
        if (item.value() == info) {
            removeUDiskDir(item.key());
            m_mapMountUdisk.remove(item.key());
            break;
        }
    }
}

/**
 * @brief
 * 添加要监听的U盘目录
 * @param path
 */
void NotifyManage::addUDiskDir(const QString &path)
{
    if (!m_uDiskDirs.contains(path))
        m_uDiskDirs.append(path);

    addDir(path);
}

void NotifyManage::removeUDiskDir(const QString &path)
{
    m_uDiskDirs.removeOne(path);
    removeDir(path);
}

/**
 * @brief
 * 添加要监听的本地目录（用户可以拷贝到本地的目录）
 * @param path
 */
void NotifyManage::addRootDir(const QString &path)
{
    m_rootDirs.append(path);
    addDir(path);
}

void NotifyManage::removeRootDir(const QString &path)
{
    m_rootDirs.removeOne(path);
    removeDir(path);
}

void NotifyManage::addDir(const QString &path)
{
    m_inotify->setNotifyObject(path);
}

void NotifyManage::removeDir(const QString &path)
{
    m_inotify->removeNotifyObject(path);
}

/**
 * @brief
 * 开始监听
 */
void NotifyManage::startWatch()
{
    m_inotify->startListen();
}

/**
 * @brief
 * 清除已超过timeout S的操作
 * @param nodeMap
 * @param timeout 单位S
 */
void NotifyManage::clearOldData(QMap<QString, FileListener::OperationNode> &nodeMap, int timeout)
{
    QStringList keys = nodeMap.keys();
    QDateTime now = QDateTime::currentDateTime().addSecs(-timeout);
    for (auto &key : keys) {
        if (nodeMap[key].m_date < now)
            nodeMap.remove(key);
    }
}

/**
 * @brief
 * 接收到操作数据
 * @param node
 */
void NotifyManage::operationHappen(FileListener::OperationNode node)
{
    // FileListener::showNode(node);
    if (node.m_operationType == IN_CLOSE_WRITE) {
        m_mapWrite[node.m_fileName] = node;
    } else if (node.m_operationType == IN_ACCESS) {
        m_mapRead[node.m_fileName] = node;
    }
}

/**
 * @brief
 * 判断是否涉及U盘目录
 * @param path1 第一个操作目录
 * @param path2 第二个操作目录
 * @return QPair<QString, QString> 为““则不是U盘目录
 */
QPair<QString, QString> NotifyManage::copyUDisk(const QString &path1, const QString &path2)
{
    QPair<QString, QString> pair;
    for (const auto &path : m_uDiskDirs) {
        if (pair.first.isEmpty() && path1.contains(path))
            pair.first = path;

        if (pair.second.isEmpty() && path2.contains(path))
            pair.second = path;
    }
    return pair;
}

/**
 * @brief 保存信息到本地记录
 *
 * @param path U盘目录
 * @param fileName 被拷贝文件
 * @param dateTime 操作时间
 * @param operation 操作类型:usb/pc
 */
void NotifyManage::saveHistory(const QString &path, const QString &fileName, qint64 dateTime, const QString &operation)
{
    FileListener::ReportNode reportNode;
    UdiskInfo &udiskInfo = m_mapMountUdisk[path];
    reportNode.m_fileName = fileName;
    reportNode.m_date = dateTime;
    reportNode.m_uDiskName = udiskInfo.m_uDiskName;
    reportNode.m_operation = operation;
    reportNode.m_productID = udiskInfo.m_productID;
    reportNode.m_vendorID = udiskInfo.m_vendorID;

    if (operation == OPERATION_PC_TO_USB)
        qDebug() << "文件【" << fileName << "】在[" << dateTime << "]被拷入" << path << operation;
    else if (operation == OPERATION_USB_TO_PC)
        qDebug() << "文件【" << fileName << "】在[" << dateTime << "]被拷出" << path << operation;

    FileListener::saveLocal(reportNode, m_userName);
}

void NotifyManage::clearUnmountDisk()
{
    QStringList mountPaths = m_mapMountUdisk.keys();
    for (auto item : mountPaths) {
        if (!QFile::exists(item)) {
            m_mapMountUdisk.remove(item);
            removeUDiskDir(item);
        }
    }
}
/**
 * @brief
 * 处理操作数据，判断是否是拷贝操作
 */
void NotifyManage::processNode()
{
    // 获取U盘挂载点
    QString mountPath;
    m_rwLock.lockForRead();
    QList<UdiskInfo> list = m_noFindUdiskList;
    m_rwLock.unlock();
    // qDebug() << "yang4444list.size()444444："<<list.size();
    for (auto &info : list) {
        // qDebug() << "yang4444444444";
        if (getMountDir(info.m_devNode.toStdString().c_str(), mountPath)) {
            m_mapMountUdisk[mountPath] = info;
            m_rwLock.lockForWrite();
            m_noFindUdiskList.removeOne(info);
            m_rwLock.unlock();
            qDebug() << "udisk mount path:" << mountPath;
            addUDiskDir(mountPath);
        }
    }

    // 清理已经移除的U盘
    clearUnmountDisk();

    bool isWrite = !m_mapWrite.isEmpty();
    bool isRead = !m_mapRead.isEmpty();

    if (isWrite && isRead) {
        QStringList write_list = m_mapWrite.keys();
        for (auto &write : write_list) {
            QFileInfo writeInfo(write);
            QString writeFileName(writeInfo.fileName());
            removeFileCopyInfo(writeFileName);

            QStringList read_list = m_mapRead.keys();
            for (auto &read : read_list) {
                QFileInfo readInfo(read);
                QString readFileName(readInfo.fileName());
                removeFileCopyInfo(readFileName);

                if (writeFileName != readFileName)
                    continue;

                if (writeInfo.absolutePath() == readInfo.absolutePath())
                    continue;

                QPair<QString, QString> pair(copyUDisk(writeInfo.absolutePath(), readInfo.absolutePath()));
                if (pair.first != pair.second) {
                    FileListener::ReportNode reportNode;
                    if (!pair.first.isEmpty()) {
                        saveHistory(pair.first, writeInfo.fileName(), m_mapWrite[write].m_date.toSecsSinceEpoch(), QString(OPERATION_PC_TO_USB));
                    }
                    if (!pair.second.isEmpty()) {
                        saveHistory(pair.second, readInfo.fileName(), m_mapRead[read].m_date.toSecsSinceEpoch(), QString(OPERATION_USB_TO_PC));
                    }
                    Q_EMIT fileChanged();
                }
                m_mapWrite.remove(write);
                m_mapRead.remove(read);
                break;
            }
        }
    }

    clearOldData(m_mapWrite, TIMEOUT_INTERVAL);
    clearOldData(m_mapRead, TIMEOUT_INTERVAL);
}
