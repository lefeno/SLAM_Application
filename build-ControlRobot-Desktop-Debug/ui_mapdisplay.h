/********************************************************************************
** Form generated from reading UI file 'mapdisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPDISPLAY_H
#define UI_MAPDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapDisplay
{
public:

    void setupUi(QWidget *MapDisplay)
    {
        if (MapDisplay->objectName().isEmpty())
            MapDisplay->setObjectName(QStringLiteral("MapDisplay"));
        MapDisplay->resize(400, 300);

        retranslateUi(MapDisplay);

        QMetaObject::connectSlotsByName(MapDisplay);
    } // setupUi

    void retranslateUi(QWidget *MapDisplay)
    {
        MapDisplay->setWindowTitle(QApplication::translate("MapDisplay", "Map Display", 0));
    } // retranslateUi

};

namespace Ui {
    class MapDisplay: public Ui_MapDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPDISPLAY_H
