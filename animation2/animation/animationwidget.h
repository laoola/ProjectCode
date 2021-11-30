
/**
 ** @file:	   animationwidget.h
 ** @author:	   浓咖啡
 ** @date:	   2015.4.15
 ** @brief:      动画窗口容器
 */

#ifndef ANIMATIONWIDGET_H
#define ANIMATIONWIDGET_H

#include "animation.h"

class AnimationWidget : public QWidget
{
    Q_OBJECT
public:
    enum ANIMATION_DIRECT{
        ANIMATION_RIGHT,  //向右触发动画
        ANIMATION_LEFT  //向左触发动画
    };
    explicit AnimationWidget(const QList<QWidget *> &list, const QString &aniName, \
                             int timeVal = 1000, const QEasingCurve &easing = QEasingCurve::OutCubic);
    AbstractAnimation *getAnimation(){return animation;}
    QWidget *getWidget(int index) const {return listWidget.at(index);}
    QWidget *currentWidget() const;
    void hideOther(int index);
    void allShow();
    void raise(int index);
    void animationShow(int index, ANIMATION_DIRECT);
    void setNewAnimation(const QString &name, int timeVal = 1000,
                         const QEasingCurve &easing = QEasingCurve::OutCubic);

signals:
    void animationFinshed();
    void animationFinshed(int);

private slots:
    void animationFinshedSlot();

private:
    AbstractAnimation *createAnimation(const QString &name, int timeVal, const QEasingCurve &easing);

    QList<QWidget *> listWidget;  //所有的子窗口链表
    int curIndex;
    AbstractAnimation *animation;
};

#endif // ANIMATIONWIDGET_H
