
/**
 ** @file:	   animationwidget.cpp
 ** @author:	   浓咖啡
 ** @date:	   2015.4.15
 ** @brief:    动画窗口容器
 */

#include "animationwidget.h"

/**
 * @brief 创建动画窗口容器
 * @param list 包含在此窗体的所有子窗体链表
 * @param aniName 动画名称:slide/cover/fly...看各个动画构造函数即可知
 * @param timeVal 动画时间,默认1000ms
 * @param easing 动画效果,默认QEasingCurve::OutCubic
 */
AnimationWidget::AnimationWidget(const QList<QWidget *> &list, const QString &aniName,
                                 int timeVal, const QEasingCurve &easing)
{
    listWidget = list;
    for(int i=0; i<list.length(); i++)
    {
        list.at(i)->setParent(this);
    }
    //默认第一个界面显示
    curIndex = 0;
    listWidget.at(list.length()-1)->raise();

    animation = createAnimation(aniName, timeVal, easing);
    assert(animation != NULL);
    connect(animation, SIGNAL(animationFinished()), this, SIGNAL(animationFinshed()));
    connect(animation, SIGNAL(animationFinished()), this, SLOT(animationFinshedSlot()));
}

/**
 * @brief 隐藏除了index窗口外的所有子窗体
 * @param index
 */
void AnimationWidget::hideOther(int index)
{
    for(int i=0; i<listWidget.length(); i++)
    {
        if(i != index)
        {
            listWidget.at(i)->hide();
        }
    }
}

/**
 * @brief 显示父窗体上的所有子窗体
 * @param index
 */
void AnimationWidget::allShow()
{
    for(int i=0; i<listWidget.length(); i++)
    {
        listWidget.at(i)->show();
    }
}

/**
 * @brief 把index代表的wig显示到最上面
 * @param index
 */
void AnimationWidget::raise(int index)
{
    listWidget.at(index)->raise();
    curIndex = index;
}

/**
 * @brief 得到当前显示的窗体
 * @param index
 */
QWidget *AnimationWidget::currentWidget() const
{
    return curIndex >= 0 ? listWidget.at(curIndex) : 0;
}

/**
 * @brief 开始动画效果
 * @param index 将要显示的界面
 * @param direct 动画出现方向
 */
void AnimationWidget::animationShow(int index, ANIMATION_DIRECT direct)
{
    if(direct == ANIMATION_RIGHT)
    {
        animation->animationShow(getWidget(index), 1, listWidget.at(curIndex));
    }
    else
    {
        animation->animationShow(getWidget(index), -1, listWidget.at(curIndex));
    }
    curIndex = index;
}

/**
 * @brief 动画结束槽函数
 */
void AnimationWidget::animationFinshedSlot()
{
    emit animationFinshed(curIndex);
}

/**
 * @brief 动画工厂
 */
AbstractAnimation *AnimationWidget::createAnimation(const QString &name, int timeVal, const QEasingCurve &easing)
{
    if(name == "slide")
    {
        return new AnimationSlide(timeVal, easing);
    }
    else if(name == "cover")
    {
        return new AnimationCover(timeVal, easing);
    }
    else if(name == "center")
    {
        return new AnimationCenter(timeVal, easing);
    }
    else if(name == "fly")
    {
        return new AnimationFly(timeVal, easing);
    }
    else if(name == "sharp")
    {
        return new AnimationSharp(timeVal, easing);
    }
    else
    {
        qDebug()<<"not support";
        return NULL;
    }
}

/**
 * @brief 设置新的动画形式,这是扩展接口,如果做成可动态选择的只需要调用这个接口即可
 */
void AnimationWidget::setNewAnimation(const QString &name, int timeVal, const QEasingCurve &easing)
{
    if(animation != NULL)
    {
        delete animation;
        animation = NULL;
    }

    animation = createAnimation(name, timeVal, easing);
    assert(animation != NULL);
    connect(animation, SIGNAL(animationFinished()), this, SIGNAL(animationFinshed()));
    connect(animation, SIGNAL(animationFinished()), this, SLOT(animationFinshedSlot()));
}
