/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     yanghongwei <yanghongwei@uniontech.com>
*
* Maintainer: yanghongwei <yanghongwei@uniontech.com>
*/

#ifndef FANOTIFY_H
#define FANOTIFY_H

#include "common.h"

#include <QThread>

class Fanotify : public QThread
{
    Q_OBJECT

public:
    explicit Fanotify(QObject *parent = nullptr);
    ~Fanotify();

    bool isValid();
    // 选择检测对象
    void setNotifyObject(const QString& path);
    void removeNotifyObject(const QString& path);
    // 将fanotify句柄加到select中开始监听
    void startListen();

Q_SIGNALS:
    void operationDir(QString dir);
    void operationHappen(FileListener::OperationNode node);

protected:
    void run();

private:
    // 获取句柄
    int getNotifyFD() ;
    // select监测事件
    int selectEvent(fd_set *rfd);
    // 处理文件操作事件类型
    void processEvent(const struct fanotify_event_metadata *metadata, int len);
    // 获取文件名跟修改它的进程名称
    QString getFileName(int fd);
    QString getProcessName(int pid);

private:
    QStringList m_paths ;
    int fanFd ;
};

#endif // FANOTIFY_H
