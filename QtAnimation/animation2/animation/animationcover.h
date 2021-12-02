
/**
 ** @file:	   animationcover.h
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     覆盖动画效果
 */

#ifndef ANIMATIONCOVER_H
#define ANIMATIONCOVER_H

#include "abstractanimation.h"

class AnimationCover : public AbstractAnimation
{
public:
    AnimationCover(int timeVal = 1000, const QEasingCurve &easing = QEasingCurve::OutCubic);
    void animationShow(QWidget *dispWig, int direct, QWidget *curWig);
};

#endif // ANIMATIONCOVER_H
