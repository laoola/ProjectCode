/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     yanghongwei <yanghongwei@uniontech.com>
*
* Maintainer: yanghongwei <yanghongwei@uniontech.com>
*/

#ifndef INOTIFY_H
#define INOTIFY_H

#include "common.h"

#include <QThread>
#include <QMap>

#include <atomic>

class Inotify : public QThread
{
    Q_OBJECT

public:
    explicit Inotify(QObject *parent = nullptr);
    ~Inotify();

    bool isValid();
    // 选择检测对象
    bool setNotifyObject(const QString& path);
    // 移除选择对象
    void removeNotifyObject(const QString& path);
    void startListen();

    void setEnableProcess(bool enable) { m_needProcess.store(enable); }

Q_SIGNALS:
    void operationHappen(FileListener::OperationNode node);

protected:
    void run();

private:
    // 获取文件操作事件类型
    int getEvent(const struct inotify_event *event);

private:
    int fd;
    QMap<QString, int> m_mapListenPathWd;
    QMap<QString, FileListener::OperationNode> m_mapRead;        // 访问操作的所有数据
    QMap<QString, FileListener::OperationNode> m_mapCopyHistory; // 拷贝历史记录用来去重，可能存在问题
    std::atomic<bool> m_needProcess;
};

#endif // INOTIFY_H
