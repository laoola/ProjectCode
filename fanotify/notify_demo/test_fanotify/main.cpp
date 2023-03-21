#include "filelisten/notify_manage.h"
#include "filelisten/fanotify_file_listener.h"

#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    qRegisterMetaType<FileListener::OperationNode>("FileListener::OperationNode");

    // NotifyManage manager;

    // manager.addRootDir("/home/");

    // // manager.addUDiskDir("/home/yang/udisk");

    // manager.startWatch();
    Fanotify notify;

    notify.setNotifyObject("/home");

    notify.start();

    return a.exec();
}