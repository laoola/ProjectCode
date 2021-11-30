
/**
 ** @file:	   animationsharp.h
 ** @author:	   浓咖啡
 ** @date:	   2015.4.18
 ** @brief:     对角变化
 */

#ifndef ANIMATIONSHARP_H
#define ANIMATIONSHARP_H

#include "abstractanimation.h"

class AnimationSharp : public AbstractAnimation
{
public:
    AnimationSharp(int timeVal = 1000, const QEasingCurve &easing = QEasingCurve::OutCubic);
    void animationShow(QWidget *dispWig, int direct, QWidget *curWig);
};

#endif // ANIMATIONSLIDE_H
