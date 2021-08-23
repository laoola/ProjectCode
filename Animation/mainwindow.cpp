#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPropertyAnimation>
#include <QApplication>
#include <QDesktopWidget>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CreateSignal();
    InitWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    int nXpos = (QApplication::desktop()->width() - this->width())/2;
    int nYpos = (QApplication::desktop()->height() - this->height())/2;

    move(nXpos, nYpos);
}

void MainWindow::CreateSignal()
{
    QObject::connect(ui->btn_prepage, SIGNAL(clicked()), this, SLOT(slotBtnPrePage()));
    QObject::connect(ui->btn_nextpage, SIGNAL(clicked()), this, SLOT(slotBtnNextPage()));
    QObject::connect(ui->btn_gopage, SIGNAL(clicked()), this, SLOT(slotBtnGoPage()));
}

void MainWindow::InitWindow()
{
    ui->label_pagecount->setText(QString("共%1页").arg(ui->stackedWidget->count()));
    ui->lineEdit_goIndex->setValidator(new QIntValidator(1, ui->stackedWidget->count(), this));
    ui->lineEdit_goIndex->setText("1");
}

void MainWindow::slotBtnPrePage()
{
    int nCount = ui->stackedWidget->count();
    int nCurent = ui->stackedWidget->currentIndex();

    ui->stackedWidget->slotPrePage();
    ui->lineEdit_goIndex->setText(QString::number(((nCurent - 1) < 0 ? (nCurent-1+nCount) % nCount : (nCurent-1) %nCount)+1));
}

void MainWindow::slotBtnNextPage()
{
    int nCount = ui->stackedWidget->count();
    int nCurent = ui->stackedWidget->currentIndex();

    ui->stackedWidget->slotNextPage();
    ui->lineEdit_goIndex->setText(QString::number(((nCurent + 1) % nCount)+1));
}

void MainWindow::slotBtnGoPage()
{
    int nGoPage = ui->lineEdit_goIndex->text().toInt()-1;

    if(nGoPage == ui->stackedWidget->currentIndex())
        return;

    ui->stackedWidget->slotSwitchPage(nGoPage);
}


