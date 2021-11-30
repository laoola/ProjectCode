#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFontDatabase>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFontDatabase fontdatabase;
    QStringList fontlist = fontdatabase.families();

    for(int i = 0; i < fontlist.size(); ++i)
    {
        qDebug()<<"No."<<i<<"  "<<fontlist.at(i);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
