/**
 ** @file:	   animationsharp.cpp
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     对角变化,这个没做好
 */

#include "animationsharp.h"

AnimationSharp::AnimationSharp(int timeVal, const QEasingCurve &easing):
    AbstractAnimation("sharp", timeVal, easing)
{
}

void AnimationSharp::animationShow(QWidget *dispWig, int direct, QWidget *curWig)
{
    int w = dispWig->width();
    int h = dispWig->height();

    QPropertyAnimation *animal1= new QPropertyAnimation(dispWig,"geometry");
    animal1->setDuration(timeVal);
    (direct > 0)? animal1->setStartValue(QRect(0,0, 0, 0)):animal1->setStartValue(QRect(w,h, 0, 0));
    (direct > 0)? animal1->setEndValue(QRect(0,0, w, h)):animal1->setEndValue(QRect(0,0, w, h));
    animal1->setEasingCurve(easing);

    assert(curWig != NULL);
    QPropertyAnimation *animal2= new QPropertyAnimation(curWig,"geometry");
    animal2->setDuration(timeVal);
    (direct > 0)? animal2->setStartValue(QRect(0,0, w, h)):animal2->setStartValue(QRect(0,0, w, h));
    (direct > 0)? animal2->setEndValue(QRect(w,h, 0, 0)):animal2->setEndValue(QRect(0,0, 0, 0));
    animal2->setEasingCurve(easing);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(animal1);
    group->addAnimation(animal2);
    connect(group, SIGNAL(finished()), this, SIGNAL(animationFinished()));
    group->start();
    dispWig->raise();
}
