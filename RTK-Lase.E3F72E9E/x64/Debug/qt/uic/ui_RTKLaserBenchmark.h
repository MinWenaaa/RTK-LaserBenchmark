/********************************************************************************
** Form generated from reading UI file 'RTKLaserBenchmark.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RTKLASERBENCHMARK_H
#define UI_RTKLASERBENCHMARK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RTKLaserBenchmarkClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RTKLaserBenchmarkClass)
    {
        if (RTKLaserBenchmarkClass->objectName().isEmpty())
            RTKLaserBenchmarkClass->setObjectName("RTKLaserBenchmarkClass");
        RTKLaserBenchmarkClass->resize(600, 400);
        menuBar = new QMenuBar(RTKLaserBenchmarkClass);
        menuBar->setObjectName("menuBar");
        RTKLaserBenchmarkClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RTKLaserBenchmarkClass);
        mainToolBar->setObjectName("mainToolBar");
        RTKLaserBenchmarkClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(RTKLaserBenchmarkClass);
        centralWidget->setObjectName("centralWidget");
        RTKLaserBenchmarkClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(RTKLaserBenchmarkClass);
        statusBar->setObjectName("statusBar");
        RTKLaserBenchmarkClass->setStatusBar(statusBar);

        retranslateUi(RTKLaserBenchmarkClass);

        QMetaObject::connectSlotsByName(RTKLaserBenchmarkClass);
    } // setupUi

    void retranslateUi(QMainWindow *RTKLaserBenchmarkClass)
    {
        RTKLaserBenchmarkClass->setWindowTitle(QCoreApplication::translate("RTKLaserBenchmarkClass", "RTKLaserBenchmark", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RTKLaserBenchmarkClass: public Ui_RTKLaserBenchmarkClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RTKLASERBENCHMARK_H
