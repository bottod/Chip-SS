/********************************************************************************
** Form generated from reading UI file 'paydlg.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAYDLG_H
#define UI_PAYDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PayDlg
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PayDlg)
    {
        if (PayDlg->objectName().isEmpty())
            PayDlg->setObjectName(QLatin1String("PayDlg"));
        PayDlg->resize(800, 600);
        menubar = new QMenuBar(PayDlg);
        menubar->setObjectName(QLatin1String("menubar"));
        PayDlg->setMenuBar(menubar);
        centralwidget = new QWidget(PayDlg);
        centralwidget->setObjectName(QLatin1String("centralwidget"));
        PayDlg->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(PayDlg);
        statusbar->setObjectName(QLatin1String("statusbar"));
        PayDlg->setStatusBar(statusbar);

        retranslateUi(PayDlg);

        QMetaObject::connectSlotsByName(PayDlg);
    } // setupUi

    void retranslateUi(QMainWindow *PayDlg)
    {
        PayDlg->setWindowTitle(QApplication::translate("PayDlg", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PayDlg: public Ui_PayDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAYDLG_H
