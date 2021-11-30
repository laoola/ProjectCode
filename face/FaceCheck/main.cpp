#include "widget.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    int rc_x = (QApplication::desktop()->width() - w.width())/2;
    int rc_y = (QApplication::desktop()->height() - w.height())/2;

    w.move (rc_x, rc_y);
    w.show();
    return a.exec();
}
