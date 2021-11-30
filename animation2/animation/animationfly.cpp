
/**
 ** @file:	   animationfly.cpp
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     飞入飞出效果
 */

#include "animationfly.h"

AnimationFly::AnimationFly(int timeVal, const QEasingCurve &easing):
    AbstractAnimation("fly", timeVal, easing)
{
}

void AnimationFly::animationShow(QWidget *dispWig, int direct, QWidget *curWig)
{
    int w = dispWig->width();
    int h = dispWig->height();
    if(direct > 0){//从左上角慢慢变大出现
        QPropertyAnimation *animal= new QPropertyAnimation(dispWig,"geometry");
        animal->setDuration(timeVal);
        animal->setStartValue(QRect(-w,-h, w, h));
        animal->setEndValue(QRect(0,0, w, h));
        animal->setEasingCurve(easing);
        connect(animal, SIGNAL(finished()), this, SIGNAL(animationFinished()));
        animal->start();
        dispWig->raise();
    }else{  //右下角慢慢变大出现
        QPropertyAnimation *animal= new QPropertyAnimation(dispWig,"geometry");
        animal->setDuration(timeVal);
        animal->setStartValue(QRect(w,h, 0, 0));
        animal->setEndValue(QRect(0,0, w, h));
        animal->setEasingCurve(easing);
        connect(animal, SIGNAL(finished()), this, SIGNAL(animationFinished()));
        animal->start();
        dispWig->raise();
    }
}
