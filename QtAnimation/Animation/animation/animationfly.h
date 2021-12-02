
/**
 ** @file:	   animationfly.h
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     飞入飞出效果
 */

#ifndef ANIMATIONFLY_H
#define ANIMATIONFLY_H

#include "abstractanimation.h"

class AnimationFly : public AbstractAnimation
{
public:
    AnimationFly(int timeVal = 1000, const QEasingCurve &easing = QEasingCurve::OutCubic);
    void animationShow(QWidget *dispWig, int direct, QWidget *curWig);
};

#endif // ANIMATIONCOVER_H
