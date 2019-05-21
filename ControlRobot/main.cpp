#include "connecttorobot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectToRobot w;
    w.show();

    return a.exec();
}
