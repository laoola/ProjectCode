
/**
 ** @file:	   animationslide.h
 ** @author:	   浓咖啡
 ** @date:	   2015.4.15
 ** @brief:     滑动效果,两个界面同时位移
 */

#ifndef ANIMATIONSLIDE_H
#define ANIMATIONSLIDE_H

#include "abstractanimation.h"

class AnimationSlide : public AbstractAnimation
{
public:
    AnimationSlide(int timeVal = 1000, const QEasingCurve &easing = QEasingCurve::OutCubic);
    void animationShow(QWidget *dispWig, int direct, QWidget *curWig = NULL);
};

#endif // ANIMATIONSLIDE_H
