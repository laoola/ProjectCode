/**
 ** @file:	   mainwindow.h
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     演示窗口
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "animationwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void initMenuBar();
    void initToolBar();
    void initCenterWidget();
    void initStatusBar();
    void initConnect();

    void showEvent(QShowEvent *event);

signals:

private slots:
    void finishSlot();

    void upSlot();
    void downSlot();

    void changeToSlide();
    void changeToCover();
    void changeToCenter();
    void changeToFly();
    void changeToSharp();

private:
    QMenu *menuAnimation;
    QAction *actSlide;
    QAction *actCover;
    QAction *actCenter;
    QAction *actFly;
    QAction *actSharp;

    QToolBar *toolBar;
    QAction *up;
    QAction *down;

    QLabel *status;

    AnimationWidget *center;
    QList<QWidget *> list;
    QWidget *wig1;
    QWidget *wig2;
    QWidget *wig3;
    int curWig;

    static const int w=510;
    static const int h=305;
};

#endif // MAINWINDOW_H
