/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     yanghongwei <yanghongwei@uniontech.com>
*
* Maintainer: yanghongwei <yanghongwei@uniontech.com>
*/

#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QMap>
#include <QDateTime>
#include <QVector>

namespace FileListener {
    struct OperationNode
    {
        QString m_fileName;
        quint32 m_operationType;
        QDateTime m_date;

        bool operator==(const OperationNode &b) {
            return ((this->m_date == b.m_date) && (this->m_fileName == b.m_fileName)
                    && (this->m_operationType == b.m_operationType));
        }
        void clear() {
            m_fileName.clear();
        }
    };

    /**
     * @brief 保存到文件的数据结构
     * m_operation: "O"->拷出 "I"->拷入
     */
    struct ReportNode
    {
        QString m_fileName;
        qint64 m_date;
        QString m_uDiskName;
        QString m_operation;
        QString m_vendorID;
        QString m_productID;
        QString m_serialID;

        bool operator==(const FileListener::ReportNode &b) {
            return ((this->m_date == b.m_date) && (this->m_fileName == b.m_fileName)
                    && (this->m_operation == b.m_operation) && (this->m_uDiskName == b.m_uDiskName)
                    && (this->m_vendorID == b.m_vendorID) && (this->m_productID == b.m_productID)
                    && (this->m_serialID == b.m_serialID));
        }
    };

    bool saveLocal(const FileListener::ReportNode &node, const QString &username);
    bool saveLocal(const QList<FileListener::ReportNode> &nodeList, const QString &username);
    bool removeLocal(FileListener::ReportNode &node, const QString &username);
    bool removeLocal(const QList<FileListener::ReportNode> &nodeList, const QString &username);
    QList<FileListener::ReportNode> readLocal(const QString &username);
    FileListener::ReportNode readOneLocal(const QString &username);

    void showNode(const FileListener::OperationNode &node);

    void removeFileCopyInfo(QString &fileName);
    QString findReadNodePath(const QString &filePath, const QMap<QString, FileListener::OperationNode> &nodeMap);
    void clearOldData(QMap<QString, FileListener::OperationNode> &nodeMap, int timeout);
}

#endif // COMMON_H
