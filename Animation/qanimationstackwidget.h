#ifndef QANIMATIONSTACKWIDGET_H
#define QANIMATIONSTACKWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>

class QAnimationStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit QAnimationStackWidget(QWidget *parent = NULL);
    ~QAnimationStackWidget();

    void paintEvent(QPaintEvent *event);
    void setDuration(int nTime);

signals:

public slots:
    //设置动画持续的间隔
    void slotValueChange(QVariant variant);
    void slotAnimationFinish();
    void slotNextPage();
    void slotPrePage();
    void slotSwitchPage(int nIndex);

private:
    void paintPrevious(QPainter &, int);
    void paintNext(QPainter &, int);

private:
    QPropertyAnimation *m_pAnimation; //动画框架
    int m_nDuration;                   //动画的持续时间
    bool m_bIsAnimation;               //是否正在动画
    QVariant       currentValue;    //被Animation动画改变的值
    int         m_nWidgetCount;        //保存当前StackWidget里面的子成员数
    int         m_nNextIndex;          //下一个要切换的索引
    bool m_bToNext;
};

#endif // QANIMATIONSTACKWIDGET_H
