/********************************************************************************
** Form generated from reading UI file 'displayapplication.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYAPPLICATION_H
#define UI_DISPLAYAPPLICATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DisplayApplication
{
public:
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DisplayApplication)
    {
        if (DisplayApplication->objectName().isEmpty())
            DisplayApplication->setObjectName(QStringLiteral("DisplayApplication"));
        DisplayApplication->resize(800, 600);
        centralwidget = new QWidget(DisplayApplication);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        DisplayApplication->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(DisplayApplication);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        DisplayApplication->setStatusBar(statusbar);

        retranslateUi(DisplayApplication);

        QMetaObject::connectSlotsByName(DisplayApplication);
    } // setupUi

    void retranslateUi(QMainWindow *DisplayApplication)
    {
        DisplayApplication->setWindowTitle(QApplication::translate("DisplayApplication", "Map Display", 0));
    } // retranslateUi

};

namespace Ui {
    class DisplayApplication: public Ui_DisplayApplication {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYAPPLICATION_H
