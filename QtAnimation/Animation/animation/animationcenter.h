
/**
 ** @file:	   animationcenter.h
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     中心隐现
 */

#ifndef ANIMATIONCENTER_H
#define ANIMATIONCENTER_H

#include "abstractanimation.h"

class AnimationCenter : public AbstractAnimation
{
public:
    AnimationCenter(int timeVal = 1000, const QEasingCurve &easing = QEasingCurve::OutCubic);
    void animationShow(QWidget *dispWig, int direct, QWidget *curWig);
};

#endif // ANIMATIONCOVER_H
