/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBox_video_0;
    QPushButton *pushButton_update;
    QPushButton *pushButton_Start;
    QPushButton *pushButton_stop;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_display;
    QSpacerItem *horizontalSpacer;
    QLabel *label_ImageDisplay_0;
    QPlainTextEdit *plainTextEdit_log_0;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(995, 623);
        gridLayout = new QGridLayout(Widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        comboBox_video_0 = new QComboBox(Widget);
        comboBox_video_0->setObjectName(QStringLiteral("comboBox_video_0"));

        horizontalLayout->addWidget(comboBox_video_0);

        pushButton_update = new QPushButton(Widget);
        pushButton_update->setObjectName(QStringLiteral("pushButton_update"));

        horizontalLayout->addWidget(pushButton_update);

        pushButton_Start = new QPushButton(Widget);
        pushButton_Start->setObjectName(QStringLiteral("pushButton_Start"));

        horizontalLayout->addWidget(pushButton_Start);

        pushButton_stop = new QPushButton(Widget);
        pushButton_stop->setObjectName(QStringLiteral("pushButton_stop"));

        horizontalLayout->addWidget(pushButton_stop);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_display = new QLabel(Widget);
        label_display->setObjectName(QStringLiteral("label_display"));
        label_display->setMaximumSize(QSize(400, 400));
        label_display->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_display);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label_ImageDisplay_0 = new QLabel(Widget);
        label_ImageDisplay_0->setObjectName(QStringLiteral("label_ImageDisplay_0"));
        label_ImageDisplay_0->setMaximumSize(QSize(400, 400));
        label_ImageDisplay_0->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_ImageDisplay_0);

        plainTextEdit_log_0 = new QPlainTextEdit(Widget);
        plainTextEdit_log_0->setObjectName(QStringLiteral("plainTextEdit_log_0"));
        plainTextEdit_log_0->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_2->addWidget(plainTextEdit_log_0);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        pushButton_update->setText(QApplication::translate("Widget", "\345\210\267\346\226\260\350\256\276\345\244\207", 0));
        pushButton_Start->setText(QApplication::translate("Widget", "\345\274\200\345\247\213\346\243\200\346\265\213", 0));
        pushButton_stop->setText(QApplication::translate("Widget", "\345\201\234\346\255\242\346\243\200\346\265\213", 0));
        label_display->setText(QApplication::translate("Widget", "\346\243\200\346\265\213\347\273\223\346\236\234\346\230\276\347\244\272", 0));
        label_ImageDisplay_0->setText(QApplication::translate("Widget", "\346\230\276\347\244\272\345\233\276\345\203\217\350\257\206\345\210\253\347\273\223\346\236\234", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
