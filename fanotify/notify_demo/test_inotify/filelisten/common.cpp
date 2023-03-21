/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     yanghongwei <yanghongwei@uniontech.com>
*/

#include "common.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

#define U_DISK_NOTIFY_LOCAL_FILE    "/home/%1/.cache/dmcs/udisk_notify_history"

namespace FileListener {

bool saveLocal(const FileListener::ReportNode &node, const QString &username)
{
    QFile file(QString(U_DISK_NOTIFY_LOCAL_FILE).arg(username));
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "udisk notify history file open error.";
        return false;
    }

    QTextStream out(&file);
    out << node.m_fileName << "=" << node.m_date << "=" << node.m_uDiskName << "=" << node.m_operation
        << "=" << node.m_vendorID << "=" << node.m_productID << "=" << node.m_serialID << "\n";

    file.close();
    return true;
}

bool saveLocal(const QList<FileListener::ReportNode> &nodeList, const QString &username)
{
    QFile file(QString(U_DISK_NOTIFY_LOCAL_FILE).arg(username));
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "udisk notify history file open error.";
        return false;
    }

    QTextStream out(&file);
    for (auto &node : nodeList) {
        out << node.m_fileName << "=" << node.m_date << "=" << node.m_uDiskName << "=" << node.m_operation
            << "=" << node.m_vendorID << "=" << node.m_productID << "=" << node.m_serialID << "\n";
    }

    file.close();
    return true;
}

bool removeLocal(FileListener::ReportNode &node, const QString &username)
{
    QFile file(QString(U_DISK_NOTIFY_LOCAL_FILE).arg(username));
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qWarning() << "udisk notify history file open error.";
        return false;
    }

    QList<FileListener::ReportNode> listNode;
    FileListener::ReportNode nodeTemp;
    do
    {
        QString stringNode(file.readLine());
        if (!stringNode.isEmpty()) {
            stringNode.remove("\n");
            QStringList infoList = stringNode.split("=");
            nodeTemp.m_fileName = infoList[0];
            nodeTemp.m_date = infoList[1].toLongLong();
            nodeTemp.m_uDiskName = infoList[2];
            nodeTemp.m_operation = infoList[3];
            nodeTemp.m_vendorID = infoList[4];
            nodeTemp.m_productID = infoList[5];
            nodeTemp.m_serialID = infoList[6];
            if (nodeTemp == node)
                continue;

            listNode.append(nodeTemp);
        }
    } while (!file.atEnd());

    file.close();
    file.remove();

    return FileListener::saveLocal(listNode, username);
}

bool removeLocal(const QList<FileListener::ReportNode> &nodeList, const QString &username)
{
    QFile file(QString(U_DISK_NOTIFY_LOCAL_FILE).arg(username));
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qWarning() << "udisk notify history file open error.";
        return false;
    }

    QList<FileListener::ReportNode> listNode;
    FileListener::ReportNode nodeTemp;
    do
    {
        QString stringNode(file.readLine());
        if (!stringNode.isEmpty()) {
            stringNode.remove("\n");
            QStringList infoList = stringNode.split("=");
            nodeTemp.m_fileName = infoList[0];
            nodeTemp.m_date = infoList[1].toLongLong();
            nodeTemp.m_uDiskName = infoList[2];
            nodeTemp.m_operation = infoList[3];
            nodeTemp.m_vendorID = infoList[4];
            nodeTemp.m_productID = infoList[5];
            nodeTemp.m_serialID = infoList[6];

            if (nodeList.contains(nodeTemp))
                continue;
            listNode.append(nodeTemp);
        }
    } while (!file.atEnd());

    file.close();
    file.remove();

    return FileListener::saveLocal(listNode, username);
}

QList<FileListener::ReportNode> readLocal(const QString &username)
{
    QFile file(QString(U_DISK_NOTIFY_LOCAL_FILE).arg(username));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "udisk notify history file open error.";
        return QList<FileListener::ReportNode>();
    }

    QList<FileListener::ReportNode> listNode;
    FileListener::ReportNode node;
    do
    {
        QString stringNode(file.readLine());
        if (!stringNode.isEmpty()) {
            stringNode.remove("\n");
            QStringList infoList = stringNode.split("=");
            node.m_fileName = infoList[0];
            node.m_date = infoList[1].toLongLong();
            node.m_uDiskName = infoList[2];
            node.m_operation = infoList[3];
            node.m_vendorID = infoList[4];
            node.m_productID = infoList[5];
            node.m_serialID = infoList[6];
            listNode.append(node);
        }
    } while (!file.atEnd());

    file.close();
    return listNode;
}

FileListener::ReportNode readOneLocal(const QString &username)
{
    QFile file(QString(U_DISK_NOTIFY_LOCAL_FILE).arg(username));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "udisk notify history file open error.";
        return FileListener::ReportNode();
    }

    FileListener::ReportNode node;
    QString stringNode(file.readLine());
    if (!stringNode.isEmpty()) {
        stringNode.remove("\n");
        QStringList infoList = stringNode.split("=");
        node.m_fileName = infoList[0];
        node.m_date = infoList[1].toLongLong();
        node.m_uDiskName = infoList[2];
        node.m_operation = infoList[3];
        node.m_vendorID = infoList[4];
        node.m_productID = infoList[5];
        node.m_serialID = infoList[6];
    }

    file.close();
    return node;
}

void showNode(const FileListener::OperationNode &node)
{
    qDebug() << "fileName:" << node.m_fileName
             << " operationType:" << node.m_operationType
             << " date:" << node.m_date;
}

// 处理复制文件使用共存模式时，复制文件添加（复制）字段，英文环境下为(copy)
void removeFileCopyInfo(QString &fileName)
{
    QString tmp = fileName;
    tmp.replace(QRegExp("\\(副本 *\\d*\\)$"),"");
    if (tmp == fileName) {
        tmp.replace(QRegExp("\\(副本 *\\d*\\)\\."),".");
    }
    tmp.replace(QRegExp("\\(copy *\\d*\\)$"),"");
    if (tmp == fileName) {
        tmp.replace(QRegExp("\\(copy *\\d*\\)\\."),".");
    }
    fileName = tmp;
}

QString findReadNodePath(const QString &filePath, const QMap<QString, OperationNode> &nodeMap)
{
    QFileInfo writeFileInfo(filePath);
    QString writeFilePath = writeFileInfo.filePath();
    QString writeFileName = writeFileInfo.fileName();
    QString writeFileNameNoCopy = writeFileName;
    removeFileCopyInfo(writeFileNameNoCopy);

    QString pathCopy;
    QString pathNoCopy;
    const QStringList &filePathList = nodeMap.keys();
    for (auto &path : filePathList) {
        QFileInfo fileInfo(path);
        if (fileInfo.filePath() == writeFilePath)
            continue;

        if (pathNoCopy.isEmpty() && fileInfo.fileName() == writeFileName) {
            pathNoCopy = path;
        }

        if (pathCopy.isEmpty() && fileInfo.fileName() == writeFileNameNoCopy) {
            pathCopy = path;
        }
    }
    return pathNoCopy.isEmpty() ? pathCopy : pathNoCopy;
}

void clearOldData(QMap<QString, FileListener::OperationNode> &nodeMap, int timeout)
{
    QStringList keys = nodeMap.keys();
    QDateTime now = QDateTime::currentDateTime().addSecs(-timeout);
    for (auto &key : keys) {
        if (nodeMap[key].m_date < now)
            nodeMap.remove(key);
    }
}
}
