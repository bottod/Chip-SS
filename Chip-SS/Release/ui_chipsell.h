/********************************************************************************
** Form generated from reading UI file 'chipsell.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHIPSELL_H
#define UI_CHIPSELL_H

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

class Ui_ChipSell
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ChipSell)
    {
        if (ChipSell->objectName().isEmpty())
            ChipSell->setObjectName(QStringLiteral("ChipSell"));
        ChipSell->resize(514, 416);
        centralWidget = new QWidget(ChipSell);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ChipSell->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ChipSell);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 514, 25));
        ChipSell->setMenuBar(menuBar);
        statusBar = new QStatusBar(ChipSell);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ChipSell->setStatusBar(statusBar);

        retranslateUi(ChipSell);

        QMetaObject::connectSlotsByName(ChipSell);
    } // setupUi

    void retranslateUi(QMainWindow *ChipSell)
    {
        ChipSell->setWindowTitle(QApplication::translate("ChipSell", "ChipSell", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChipSell: public Ui_ChipSell {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHIPSELL_H
