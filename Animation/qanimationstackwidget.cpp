#include "qanimationstackwidget.h"

#include <QPainter>

QAnimationStackWidget::QAnimationStackWidget(QWidget *parent)
{
    m_bIsAnimation = false;
    //设置默认的时间间隔
    m_nDuration = 1000;
    //初始化m_pAnimation框架、并连接信号和槽
    m_pAnimation = new QPropertyAnimation(this, QByteArray());
    connect(m_pAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(slotValueChange(QVariant)));
    connect(m_pAnimation, SIGNAL(finished()), this, SLOT(slotAnimationFinish()));
}

QAnimationStackWidget::~QAnimationStackWidget()
{

}

void QAnimationStackWidget::paintEvent(QPaintEvent *event)
{
    if( m_bIsAnimation )
    {
        QPainter paint(this);

        //绘制当前Widget
        paintPrevious(paint, currentIndex());
        //绘制下一个widget
        paintNext(paint, m_nNextIndex);
    }
}

void QAnimationStackWidget::slotValueChange(QVariant variant)
{
    currentValue = variant;
    update();
}

void QAnimationStackWidget::slotAnimationFinish()
{
    m_bIsAnimation = false;
    widget(currentIndex())->show();
    setCurrentIndex(m_nNextIndex);
}

void QAnimationStackWidget::slotNextPage()
{
    //如果正在动画，那么return
    if( m_bIsAnimation )
    {
        return;
    }
    m_bIsAnimation = true;
    m_bToNext = false;
    m_nWidgetCount = count();
    int c = currentIndex();
    //计算下一页的索引
    m_nNextIndex = (c + 1) % m_nWidgetCount;
    //隐藏当前的widget
    widget(c)->hide();
    //开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    int x = g.x();
    int width = g.width();
    m_pAnimation->setStartValue(width);
    m_pAnimation->setEndValue(0);
    m_pAnimation->setDuration(m_nDuration);
    m_pAnimation->start();
}

void QAnimationStackWidget::slotPrePage()
{
    //如果正在动画，那么return
    if( m_bIsAnimation )
    {
        return;
    }
    m_bIsAnimation = true;
    m_bToNext = true;
    m_nWidgetCount = count();
    int c = currentIndex();
    //计算下一页的索引
    m_nNextIndex =  (c - 1) < 0 ? (c-1+m_nWidgetCount) % m_nWidgetCount : (c-1) %m_nWidgetCount;
    //隐藏当前的widget
    widget(c)->hide();
    //开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    int x = g.x();
    int width = g.width();
    m_pAnimation->setStartValue(width);
    m_pAnimation->setEndValue(0);
    m_pAnimation->setDuration(m_nDuration);
    m_pAnimation->start();
}

void QAnimationStackWidget::slotSwitchPage(int nIndex)
{
    //如果正在动画，那么return
    if( m_bIsAnimation )
    {
        return;
    }
    m_bIsAnimation = true;
    m_nWidgetCount = count();
    int c = currentIndex();
    //计算下一页的索引
    m_nNextIndex = nIndex;
    m_bToNext = m_nNextIndex > c ? true : false;
    //隐藏当前的widget
    widget(c)->hide();
    //开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    int x = g.x();
    int width = g.width();
    m_pAnimation->setStartValue(width);
    m_pAnimation->setEndValue(0);
    m_pAnimation->setDuration(m_nDuration);
    m_pAnimation->start();
}

void QAnimationStackWidget::paintPrevious(QPainter &paint, int nCurrentIndex)
{
    //获得当前页面的Widget
    QWidget *w = widget(nCurrentIndex);
    QPixmap pixmap(w->size());
    //将Widget的内容渲染到QPixmap对象中，即将Widget变成一张图片
    w->render(&pixmap);
    QRect r = w->geometry();
    //绘制当前的Widget
    double value = currentValue.toDouble();
    QRectF r1(0.0, 0.0, value, r.height());
    QRectF r2(r.width() - value, 0, value, r.height());

    if(m_bToNext)
    {
        paint.drawPixmap(r1, pixmap, r2);
    }
    else
    {
        paint.drawPixmap(r2, pixmap, r1);
    }
}

void QAnimationStackWidget::paintNext(QPainter &paint, int m_nNextIndex)
{
    QWidget *nextWidget = widget(m_nNextIndex);
    QRect r = geometry();
    //这行代码不加会有bug，第一次切换的时候，QStackedWidget并没有为child分配大小
    nextWidget->resize(r.width(), r.height());
    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);
    double value = currentValue.toDouble();
    QRectF r1(value, 0.0, r.width() - value, r.height());
    QRectF r2(0.0, 0.0, r.width() - value, r.height());

    if(m_bToNext)
    {
        paint.drawPixmap(r1, nextPixmap, r2);
    }
    else
    {
        paint.drawPixmap(r2, nextPixmap, r1);
    }
}
