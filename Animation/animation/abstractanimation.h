
/**
 ** @file:	   abstractanimation.h
 ** @author:	   浓咖啡
 ** @date:	   2015.4.15
 ** @brief:      动画抽象类
 */

#ifndef ABSTRACTANIMATION_H
#define ABSTRACTANIMATION_H

#include <QtWidgets>
#include <QEasingCurve>
#include "assert.h"

class AbstractAnimation : public QObject
{
    Q_OBJECT
public:
    explicit AbstractAnimation(const QString &name, int timeVal = 1000, const QEasingCurve &easing = QEasingCurve::OutCubic);
    virtual void animationShow(QWidget *dispWig, int direct, QWidget *curWig) = 0;
    QString getName() const {return name;}
    QMutex * getMutex() const {return mutex;}
    bool tryLock() {return mutex->tryLock();}
    void unlock(){mutex->unlock();}
signals:
    void animationFinished();

public slots:

protected:
    QString name;
    QWidget *parent;
    QMutex *mutex;
    QEasingCurve easing;
    int timeVal;
};

#endif // ABSTRACTANIMATION_H
