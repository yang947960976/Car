/********************************************************************************
** Form generated from reading UI file 'setdate.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETDATE_H
#define UI_SETDATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Setdate
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_set_time;

    void setupUi(QDialog *Setdate)
    {
        if (Setdate->objectName().isEmpty())
            Setdate->setObjectName(QStringLiteral("Setdate"));
        Setdate->resize(733, 390);
        pushButton = new QPushButton(Setdate);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(654, 8, 70, 30));
        pushButton_set_time = new QPushButton(Setdate);
        pushButton_set_time->setObjectName(QStringLiteral("pushButton_set_time"));
        pushButton_set_time->setGeometry(QRect(70, 90, 80, 80));

        retranslateUi(Setdate);

        QMetaObject::connectSlotsByName(Setdate);
    } // setupUi

    void retranslateUi(QDialog *Setdate)
    {
        Setdate->setWindowTitle(QApplication::translate("Setdate", "Dialog", 0));
        pushButton->setText(QApplication::translate("Setdate", "\350\277\224\345\233\236", 0));
        pushButton_set_time->setText(QApplication::translate("Setdate", "\346\227\266\351\227\264", 0));
    } // retranslateUi

};

namespace Ui {
    class Setdate: public Ui_Setdate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETDATE_H
