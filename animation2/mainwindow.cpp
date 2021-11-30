/**
 ** @file:	   mainwindow.cpp
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     演示窗口
 */

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    initMenuBar();
    initToolBar();
    initCenterWidget();
    initStatusBar();
    initConnect();
}

void MainWindow::initMenuBar()
{
    menuAnimation = new QMenu("动画样式", this);
    actSlide = menuAnimation->addAction("滑动");
    actCover = menuAnimation->addAction("覆盖");
    actCenter = menuAnimation->addAction("中心进入");
    actFly = menuAnimation->addAction("飞入");
    actSharp = menuAnimation->addAction("对角");

    QMenuBar *bar = menuBar();
    bar->addMenu(menuAnimation);
}

void MainWindow::initToolBar()
{
    toolBar = new QToolBar(this);
    up = new QAction(QString("上一页"), toolBar);
    down = new QAction(QString("下一页"), toolBar);

    toolBar->addAction(up);
    toolBar->addAction(down);
    addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::initCenterWidget()
{
    wig1 = new QWidget();
    wig1->setStyleSheet("border-image: url(:/Res/1.png);");
    wig1->resize(w, h);
    wig2 = new QWidget();
    wig2->setStyleSheet("border-image: url(:/Res/2.png)");
    wig2->resize(w, h);
    wig3 = new QWidget();
    wig3->setStyleSheet("border-image: url(:/Res/3.png)");
    wig3->resize(w, h);

    list.append(wig1);
    list.append(wig2);
    list.append(wig3);

    //默认动画是slide
    center = new AnimationWidget(list, "slide");
    center->raise(0);
    curWig = 0;
    center->setFixedSize(w, h);
    setCentralWidget(center);
}

void MainWindow::initStatusBar()
{
    QStatusBar *bar = statusBar();
    status = new QLabel("slide");
    status->setFrameShadow(QFrame::Sunken);
    status->setFrameShape(QFrame::WinPanel);
    bar->addWidget(status);
}

void MainWindow::initConnect()
{
    connect(center, SIGNAL(animationFinshed()), this, SLOT(finishSlot()));

    connect(up, SIGNAL(triggered()), this, SLOT(upSlot()));
    connect(down, SIGNAL(triggered()), this, SLOT(downSlot()));

    connect(actSlide, SIGNAL(triggered()), this, SLOT(changeToSlide()));
    connect(actCover, SIGNAL(triggered()), this, SLOT(changeToCover()));
    connect(actCenter, SIGNAL(triggered()), this, SLOT(changeToCenter()));
    connect(actFly, SIGNAL(triggered()), this, SLOT(changeToFly()));
    connect(actSharp, SIGNAL(triggered()), this, SLOT(changeToSharp()));
}

void MainWindow::upSlot()
{
    if(curWig <= 0)
    {
        return;
    }
    //获取锁，保证动画完成后才进行下次动作
    if(center->getAnimation()->tryLock())
    {
        curWig--;
        center->animationShow(curWig, AnimationWidget::ANIMATION_LEFT);
    }
}

void MainWindow::downSlot()
{
    if(curWig >= list.length()-1)
    {
        return;
    }
    if(center->getAnimation()->tryLock())
    {
        curWig++;
        center->animationShow(curWig, AnimationWidget::ANIMATION_RIGHT);
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    int nXpos = (QApplication::desktop()->width() - this->width())/2;
    int nYpos = (QApplication::desktop()->height() - this->height())/2;

    move(nXpos, nYpos);
}

void MainWindow::finishSlot()
{
    center->getAnimation()->unlock();
}

void MainWindow::changeToSlide()
{
    center->setNewAnimation("slide");
    status->setText("slide");
}

void MainWindow::changeToCover()
{
    center->setNewAnimation("cover");
    status->setText("cover");
}

void MainWindow::changeToCenter()
{
    center->setNewAnimation("center");
    status->setText("center");
}

void MainWindow::changeToFly()
{
    center->setNewAnimation("fly");
    status->setText("fly");
}


void MainWindow::changeToSharp()
{
    center->setNewAnimation("sharp");
    status->setText("sharp");
}
