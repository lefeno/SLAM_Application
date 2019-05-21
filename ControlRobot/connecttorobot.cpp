#include "connecttorobot.h"
#include "ui_connecttorobot.h"
#include <QTextStream>

using namespace std;
ConnectToRobot::ConnectToRobot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConnectToRobot)
{
    ui->setupUi(this);
    client = new Client();
    display = new MapDisplay();
    display->setAttribute(Qt::WA_DeleteOnClose);
}

ConnectToRobot::~ConnectToRobot()
{
    delete ui;
}

void ConnectToRobot::connectToClient()
{
    switch (client->connect2Server(IP,port)){
    case (-1):
        ui->resultLabel->setText("Socket creation error");
        break;
    case (-2):
        ui->resultLabel->setText("Invalid address/ Address not supported");
        break;
    case (-3):
        ui->resultLabel->setText("Connection Failed");
        break;
    case (0):
        ui->resultLabel->setText("Connect success");
        client->sessionInit("pass1234");
        this->hide();
        display->show();
        break;
    default:
        ui->resultLabel->setText("Unknown Error");
        break;
    }
}

void ConnectToRobot::on_connectButton_clicked()
{
    IP = ui->ipRobot->text();
    port = ui->portRobot->text();
    connectToClient();
}
