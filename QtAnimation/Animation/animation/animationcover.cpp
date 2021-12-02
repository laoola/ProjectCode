
/**
 ** @file:	   animationcover.cpp
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     覆盖动画效果
 */

#include "animationcover.h"

AnimationCover::AnimationCover(int timeVal, const QEasingCurve &easing):
    AbstractAnimation("cover", timeVal, easing)
{
}

void AnimationCover::animationShow(QWidget *dispWig, int direct, QWidget *curWig)
{
    int width = dispWig->width();
    int startx = (direct > 0)? -width : width;
    QPropertyAnimation *animal= new QPropertyAnimation(dispWig,"pos");
    animal->setDuration(timeVal);
    animal->setStartValue(QPoint(startx,0));
    animal->setEndValue(QPoint(0,0));
    animal->setEasingCurve(easing);
    connect(animal, SIGNAL(finished()), this, SIGNAL(animationFinished()));
    dispWig->raise();
    animal->start();
}
