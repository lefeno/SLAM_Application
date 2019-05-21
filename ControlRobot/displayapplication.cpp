#include "displayapplication.h"
#include "ui_displayapplication.h"

DisplayApplication::DisplayApplication(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayApplication)
{
    ui->setupUi(this);
}

DisplayApplication::~DisplayApplication()
{
    delete ui;
}
