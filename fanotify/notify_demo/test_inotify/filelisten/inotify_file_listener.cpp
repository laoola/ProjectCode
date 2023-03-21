/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     yanghongwei <yanghongwei@uniontech.com>
*
* Maintainer: yanghongwei <yanghongwei@uniontech.com>
*/

#include "inotify_file_listener.h"

#include <QDebug>
#include <QDateTime>
#include <QFileInfo>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

Inotify::Inotify(QObject *parent)
    : QThread(parent)
    , m_needProcess(false)
{
    fd = inotify_init();
}

Inotify::~Inotify()
{
    close(fd);
}

bool Inotify::isValid()
{
    return fd > 0;
}

bool Inotify::setNotifyObject(const QString &path)
{
    if (m_mapListenPathWd.contains(path)) {
        return true;
    }

    uint32_t events = 0;
    events |= IN_ISDIR;
    events |= IN_ALL_EVENTS;

    int wd = inotify_add_watch(fd, path.toLocal8Bit().data(), events);
    if (wd < 0)
        return false;

    m_mapListenPathWd[path] = wd;

    return true;
}

/**
 * @brief 移除某个目录及其子目录的监测
 * @param path 需要移除的目录
 */
void Inotify::removeNotifyObject(const QString &path)
{
    QStringList listRemvoePath;
    for (auto &notifyPath : m_mapListenPathWd.keys()) {
        if (notifyPath.contains(path)) {
            inotify_rm_watch(fd, m_mapListenPathWd[notifyPath]);
            listRemvoePath.append(notifyPath);
        }
    }

    for (auto &notifyPath : listRemvoePath) {
        m_mapListenPathWd.remove(notifyPath);
    }
}

void Inotify::startListen()
{
    start();
}

void Inotify::run()
{
    int len;
    char buf[BUFSIZ];
    struct inotify_event *event;

    buf[sizeof(buf) - 1] = 0;
    QTime time;
    QTime timeClearHistory;
    time.start();
    timeClearHistory.start();
    while ((len = read(fd, buf, sizeof(buf) - 1)) > 0) {
        int nread = 0;
        while (len > 0) {
            event = (struct inotify_event *)&buf[nread];
            getEvent(event);
            nread = nread + sizeof(struct inotify_event) + event->len;
            len = len - sizeof(struct inotify_event) - event->len;
        }
        // 历史记录保存3S作为去重判断
        if (timeClearHistory.elapsed() > 3) {
            FileListener::clearOldData(m_mapCopyHistory, 3);
            timeClearHistory.restart();
        }
        if (time.elapsed() > 20) {
            FileListener::clearOldData(m_mapRead, 10);
            time.restart();
        }
    }
    qWarning() << "inotify thread is stop....";
}

int Inotify::getEvent(const inotify_event *event)
{
    if (event->len <= 0)
        return -1;

    FileListener::OperationNode node;
    QString path = m_mapListenPathWd.key(event->wd);
    node.m_fileName = QString("%1/%2").arg(path).arg(event->name);
    node.m_date = QDateTime::currentDateTime();
    node.m_operationType = 0;

    if (event->mask & IN_ISDIR) {
        if (event->mask & IN_DELETE) {
            removeNotifyObject(node.m_fileName);
        } else {
            setNotifyObject(node.m_fileName);
        }
        return 0;
    }

    // 未挂载U盘不做处理
    if (!m_needProcess.load())
        return 0;

    // 隐藏目录不做处理
    if (path.contains("/."))
        return 0;

    if (event->mask & IN_OPEN) {
        node.m_operationType = IN_OPEN;
    }

    if (event->mask & IN_ACCESS) {
        node.m_operationType = IN_ACCESS;
        m_mapRead[node.m_fileName] = node;
    }

    if (event->mask & IN_CLOSE_WRITE) {
        node.m_operationType = IN_CLOSE_WRITE;
        QString readNodeFile = FileListener::findReadNodePath(node.m_fileName, m_mapRead);
        if (!readNodeFile.isEmpty() && !m_mapCopyHistory.contains(node.m_fileName)) {
            m_mapCopyHistory[node.m_fileName] = node;
            emit operationHappen(m_mapRead[readNodeFile]);
            emit operationHappen(node);
        }
    }

    if (event->mask & IN_CLOSE_NOWRITE) {
        node.m_operationType = IN_CLOSE_NOWRITE;
    }

    if (event->mask & IN_MODIFY) {
        node.m_operationType = IN_MODIFY;
    }

    if (event->mask & IN_ATTRIB) {
        node.m_operationType = IN_ATTRIB;
    }

    if (event->mask & IN_MOVED_TO) {
        node.m_operationType = IN_MOVED_TO;
    }

    if (event->mask & IN_MOVED_FROM) {
        node.m_operationType = IN_MOVED_FROM;
    }

    if (event->mask & IN_CREATE) {
        node.m_operationType = IN_CREATE;
    }

    if (event->mask & IN_DELETE) {
        node.m_operationType = IN_DELETE;
    }

    if (event->mask & IN_DELETE_SELF) {
        node.m_operationType = IN_DELETE_SELF;
    }

    if (event->mask & IN_MOVE_SELF) {
        node.m_operationType = IN_MOVE_SELF;
    }

    return 0;
}
