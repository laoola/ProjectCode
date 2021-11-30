
/**
 ** @file:	   animationcenter.cpp
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     中心隐现
 */

#include "animationcenter.h"

AnimationCenter::AnimationCenter(int timeVal, const QEasingCurve &easing):
    AbstractAnimation("center", timeVal, easing)
{
}

void AnimationCenter::animationShow(QWidget *dispWig, int direct, QWidget *curWig)
{
    int w = dispWig->width();
    int h = dispWig->height();
    QPropertyAnimation *animal= new QPropertyAnimation(dispWig,"geometry");
    animal->setDuration(timeVal);
    animal->setStartValue(QRect(w/2,h/2, 0, 0));
    animal->setEndValue(QRect(0,0, w, h));
    animal->setEasingCurve(easing);
    connect(animal, SIGNAL(finished()), this, SIGNAL(animationFinished()));
    dispWig->raise();
    animal->start();
}
