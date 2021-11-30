
/**
 ** @file:	   abstractanimation.cpp
 ** @author:	   浓咖啡
 ** @date:	   2015.4.15
 ** @brief:      动画抽象类
 */

#include "abstractanimation.h"

/**
 * @brief 构造函数
 * @param aniName 动画名称:slide/cover/fly...看各个动画构造函数即可知
 * @param timeVal 动画时间,默认200ms
 * @param easing 动画效果,默认QEasingCurve::OutCubic
 */
AbstractAnimation::AbstractAnimation(const QString &name, int timeVal, const QEasingCurve &easing)
{
    mutex = new QMutex;
    this->name = name;
    this->timeVal = timeVal;
    this->easing = easing;
}
