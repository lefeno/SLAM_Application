#ifndef CONNECTTOROBOT_H
#define CONNECTTOROBOT_H

#include "client.h"
#include <QMainWindow>
#include <QTcpSocket>
#include "mapdisplay.h"
#include <QWidget>

namespace Ui {
class ConnectToRobot;
}

class ConnectToRobot : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectToRobot(QWidget *parent = nullptr);
    ~ConnectToRobot();

private slots:
    void on_connectButton_clicked();
    void connectToClient();

private:
    Ui::ConnectToRobot *ui;
    QString IP;
    QString port;
    Client *client;
//    DisplayApplication *display;
    MapDisplay *display;
};

#endif // CONNECTTOROBOT_H
