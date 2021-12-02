/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout1;
    QFrame *frame1;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout2;
    QLabel *label_check;
    QLabel *mainlabel;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1023, 381);
        gridLayoutWidget_3 = new QWidget(Widget);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(0, 20, 801, 351));
        gridLayout1 = new QGridLayout(gridLayoutWidget_3);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QStringLiteral("gridLayout1"));
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        frame1 = new QFrame(gridLayoutWidget_3);
        frame1->setObjectName(QStringLiteral("frame1"));
        frame1->setMinimumSize(QSize(600, 0));
        frame1->setStyleSheet(QStringLiteral("background-color: rgb(186, 189, 182);"));
        frame1->setFrameShape(QFrame::Box);
        frame1->setFrameShadow(QFrame::Sunken);
        frame1->setLineWidth(3);
        gridLayoutWidget_2 = new QWidget(frame1);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(20, 10, 761, 311));
        gridLayout2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout2->setSpacing(6);
        gridLayout2->setContentsMargins(11, 11, 11, 11);
        gridLayout2->setObjectName(QStringLiteral("gridLayout2"));
        gridLayout2->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout2->setContentsMargins(1, 1, 1, 1);
        label_check = new QLabel(gridLayoutWidget_2);
        label_check->setObjectName(QStringLiteral("label_check"));
        label_check->setMinimumSize(QSize(342, 309));
        label_check->setMaximumSize(QSize(309, 16777215));
        label_check->setAlignment(Qt::AlignCenter);

        gridLayout2->addWidget(label_check, 0, 1, 1, 1);

        mainlabel = new QLabel(gridLayoutWidget_2);
        mainlabel->setObjectName(QStringLiteral("mainlabel"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainlabel->sizePolicy().hasHeightForWidth());
        mainlabel->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(239, 41, 41, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush2(QColor(190, 190, 190, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        mainlabel->setPalette(palette);
        QFont font;
        font.setPointSize(18);
        mainlabel->setFont(font);
        mainlabel->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        mainlabel->setAlignment(Qt::AlignCenter);

        gridLayout2->addWidget(mainlabel, 0, 0, 1, 1);


        gridLayout1->addWidget(frame1, 0, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        label_check->setText(QApplication::translate("Widget", "TextLabel", Q_NULLPTR));
        mainlabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
