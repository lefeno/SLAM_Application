/********************************************************************************
** Form generated from reading UI file 'connecttorobot.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTTOROBOT_H
#define UI_CONNECTTOROBOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConnectToRobot
{
public:
    QWidget *centralWidget;
    QLineEdit *ipRobot;
    QLabel *label;
    QLineEdit *portRobot;
    QLabel *label_2;
    QPushButton *connectButton;
    QLabel *resultLabel;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ConnectToRobot)
    {
        if (ConnectToRobot->objectName().isEmpty())
            ConnectToRobot->setObjectName(QStringLiteral("ConnectToRobot"));
        ConnectToRobot->resize(251, 198);
        centralWidget = new QWidget(ConnectToRobot);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ipRobot = new QLineEdit(centralWidget);
        ipRobot->setObjectName(QStringLiteral("ipRobot"));
        ipRobot->setGeometry(QRect(40, 30, 171, 21));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 10, 121, 16));
        portRobot = new QLineEdit(centralWidget);
        portRobot->setObjectName(QStringLiteral("portRobot"));
        portRobot->setGeometry(QRect(40, 80, 171, 21));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 60, 67, 17));
        connectButton = new QPushButton(centralWidget);
        connectButton->setObjectName(QStringLiteral("connectButton"));
        connectButton->setGeometry(QRect(80, 120, 89, 25));
        resultLabel = new QLabel(centralWidget);
        resultLabel->setObjectName(QStringLiteral("resultLabel"));
        resultLabel->setGeometry(QRect(40, 150, 171, 31));
        ConnectToRobot->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(ConnectToRobot);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ConnectToRobot->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ConnectToRobot);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ConnectToRobot->setStatusBar(statusBar);

        retranslateUi(ConnectToRobot);

        QMetaObject::connectSlotsByName(ConnectToRobot);
    } // setupUi

    void retranslateUi(QMainWindow *ConnectToRobot)
    {
        ConnectToRobot->setWindowTitle(QApplication::translate("ConnectToRobot", "Login", 0));
        ipRobot->setText(QApplication::translate("ConnectToRobot", "10.42.0.83", 0));
        label->setText(QApplication::translate("ConnectToRobot", "Robot IP Adress:", 0));
        portRobot->setText(QApplication::translate("ConnectToRobot", "59998", 0));
        label_2->setText(QApplication::translate("ConnectToRobot", "Port:", 0));
        connectButton->setText(QApplication::translate("ConnectToRobot", "Connect", 0));
        resultLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ConnectToRobot: public Ui_ConnectToRobot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTTOROBOT_H
