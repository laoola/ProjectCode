/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     yanghongwei <yanghongwei@uniontech.com>
*
* Maintainer: yanghongwei <yanghongwei@uniontech.com>
*/

#include "fanotify_file_listener.h"

#include <QFileInfo>
#include <QDebug>
#include <QDateTime>

#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <iostream>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/fanotify.h>
#include <sys/inotify.h>

Fanotify::Fanotify(QObject *parent)
    : QThread(parent)
{
    // 初始化，第一个参数为FAN_CLASS_CONTENT（表示允许接收通知文件事件）另一个值为FAN_CLASS_NOTIF 为默认值。只允许接收事件通知，在一般的监听中，使用FAN_CLASS_CONTENT FAN_CLASS_PRE_CONTENT
    // 第二个参数，表示接收到操作文件事件后，本程序对文件的操作为可读可写，因为metadata中的fd为程序操作发生事件的文件的fd，可以直接操作那个文件，操作完后，要关掉。
    fanFd = fanotify_init(FAN_CLASS_NOTIF, O_RDONLY);
    if (fanFd < 0) {
        qWarning() << "init fanotiy failed.";
        return;
    }
    qInfo() << "fanotiy init success.";
}

Fanotify::~Fanotify()
{
    close(fanFd);
}

bool Fanotify::isValid()
{
    return fanFd > 0;
}

// 设置监控对象为目录下的子文件
void Fanotify::setNotifyObject(const QString& path)
{
    if (m_paths.contains(path))
        return;

    m_paths.append(path);
    // FAN_EVENT_ON_CHILD 作用于当前操作目录的所有子文件 FAN_ALL_EVENTS | FAN_ONDIR | FAN_EVENT_ON_CHILD | FAN_ALL_PERM_EVENTS
    // FAN_MARK_ADD 添加监听事件标志的标志| FAN_MARK_MOUNT
    int ret = fanotify_mark(fanFd, FAN_MARK_ADD | FAN_MARK_MOUNT, FAN_ACCESS | FAN_CLOSE_WRITE | FAN_ONDIR | FAN_EVENT_ON_CHILD,
                            AT_FDCWD, path.toLocal8Bit());
    if (ret < 0) {
        qWarning() << "listen path[" << path << "] failed.";
    }
}

/**
 * @brief 移除某个目录的监测
 * @param path 需要移除的目录
 */
void Fanotify::removeNotifyObject(const QString &path)
{
    int ret = fanotify_mark(fanFd, FAN_MARK_REMOVE | FAN_MARK_MOUNT, FAN_ACCESS | FAN_CLOSE_WRITE | FAN_ONDIR | FAN_EVENT_ON_CHILD,
                    AT_FDCWD, path.toLocal8Bit());
    if (ret < 0)
        qWarning() << "remove listen path[" << path << "] failed.";

    m_paths.removeOne(path);
}

int Fanotify::getNotifyFD()
{
    return fanFd ;
}

void Fanotify::startListen()
{
    start();
}

void Fanotify::run()
{
    char buf[4096];
    int len = 0;
    fd_set rfd;
    // 使用select监听
    FD_ZERO(&rfd);
    FD_SET(fanFd, &rfd);
    selectEvent(&rfd);

    while ((len = read(fanFd, buf, sizeof(buf))) > 0) {
        struct fanotify_event_metadata *metadata;
        metadata = (fanotify_event_metadata *)buf;
        if (metadata->fd >= 0) {
            processEvent(metadata, len);
        }
        selectEvent(&rfd) ;
    }
}

int Fanotify::selectEvent(fd_set *rfd)
{
    while (select(fanFd + 1, rfd, NULL, NULL, NULL) < 0) {
        if (errno != EINTR) {
            exit(0);
        }
    }
    return 1;
}

void Fanotify::processEvent(const struct fanotify_event_metadata *metadata, int len)
{
    while (FAN_EVENT_OK(metadata, len)) {
        // 处理matadata
        QString filePath = getFileName(metadata->fd);
        if (filePath != "unknown") {
            QString process = "";
            //getProcessName(metadata->pid);
            //if ("dde-file-manage" == process)
             {
                if (filePath.contains("890890.txt")) {
                    qDebug() << "file path:" << filePath;
                    qDebug() << "process name:" << process;
                    printf("mask：%x\n",metadata->mask);
                }
                FileListener::OperationNode node;
                node.m_fileName = filePath;
                node.m_date = QDateTime::currentDateTime();
                //qDebug() << "file path:" << filePath;
                 
      
                // if (metadata->mask & FAN_CLOSE_WRITE) {
                //      qInfo() <<"FAN_CLOSE_WRITE";
                //     node.m_operationType = IN_CLOSE_WRITE;
                // }

                // if (metadata->mask & FAN_ACCESS) {
                //      qInfo() <<"FAN_ACCESS";
                //     node.m_operationType = IN_ACCESS;
                // }

                QString strEvent = "";
                bool bDebug = false;
                if (metadata->mask & FAN_CLOSE_WRITE)
                {
                    bDebug = true;
                    strEvent = "CLOSE_WRITE";
                }

                if (metadata->mask & FAN_MODIFY)
                {
                    bDebug = true;
                    strEvent = "MODIFY";
                }

                if (bDebug)
                    printf("%s %d %s\n", strEvent.toStdString().c_str(), metadata->pid, filePath.toStdString().c_str());

                emit operationHappen(node);
             }
        }

        // 必须关闭fd
        close(metadata->fd);
        metadata = FAN_EVENT_NEXT(metadata, len) ;
    }
}

QString Fanotify::getFileName(int fd)
{
    /* Retrieve and print pathname of the accessed file. */
    char path[PATH_MAX];
    ssize_t path_len;
    char procfd_path[PATH_MAX];
    snprintf(procfd_path, sizeof(procfd_path), "/proc/self/fd/%d", fd);
    path_len = readlink(procfd_path, path, sizeof(path) - 1);
    if (path_len == -1) {
        qWarning() << "readlink error!";
        exit(EXIT_FAILURE);
    }

    path[path_len] = '\0';
    return QString(path);
}

QString Fanotify::getProcessName(int pid)
{
    if (!QFileInfo(QString("/proc/%1/status").arg(pid)).exists()) {
        return QString("unknown");
    }

    char buf[521] = { 0 };
    sprintf(buf, "grep 'Name:' /proc/%d/status |awk '{print($2)}'", pid);
    /*执行预先设定的命令，并读出该命令的标准输出*/
    FILE *fp = popen(buf, "r");
    if (NULL == fp) {
        qWarning() << "popen执行失败！";
        return QString("unknown");
    }

    memset(buf, 0, sizeof(buf));
    bool isGet = (fgets(buf, sizeof(buf), fp) != NULL);
    /*等待命令执行完毕并关闭管道及文件指针*/
    pclose(fp);

    char *find = strchr(buf, '\n');  // 找出buf中的"\n"
    if(find)
        *find = '\0';   // 替换

    return isGet ? QString(buf) : QString("unknown");
}
