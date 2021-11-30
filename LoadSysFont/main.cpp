#include <QCoreApplication>
#include <QFontDatabase>
#include <QDebug>
#include <QProcess>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QProcess process;
    process.start("/opt/vrv/cems/scommand /opt/vrv/cems/registerui");
    process.waitForFinished();


    return a.exec();
}
