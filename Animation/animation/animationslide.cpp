
/**
 ** @file:	   animationslide.cpp
 ** @author:	   浓咖啡
 ** @date:	   2015.4.15
 ** @brief:     滑动效果,两个界面同时位移
 */

#include "animationslide.h"

AnimationSlide::AnimationSlide(int timeVal, const QEasingCurve &easing):
    AbstractAnimation("slide", timeVal, easing)
{
}

void AnimationSlide::animationShow(QWidget *dispWig, int direct, QWidget *curWig)
{
    assert(curWig != NULL);
    int width = dispWig->width();
    int startx = (direct > 0)? -width : width;

    QPropertyAnimation *animal1= new QPropertyAnimation(dispWig,"pos");
    animal1->setDuration(timeVal);
    animal1->setStartValue(QPoint(startx,0));
    animal1->setEndValue(QPoint(0,0));
    animal1->setEasingCurve(easing);

    QPropertyAnimation *animal2= new QPropertyAnimation(curWig,"pos");
    animal2->setDuration(timeVal);
    animal2->setStartValue(QPoint(0,0));
    animal2->setEndValue(QPoint(-startx,0));
    animal2->setEasingCurve(easing);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(animal1);
    group->addAnimation(animal2);
    connect(group, SIGNAL(finished()), this, SIGNAL(animationFinished()));
    group->start();
    dispWig->raise();
}
