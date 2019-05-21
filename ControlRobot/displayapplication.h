#ifndef DISPLAYAPPLICATION_H
#define DISPLAYAPPLICATION_H

#include <QMainWindow>

namespace Ui {
class DisplayApplication;
}

class DisplayApplication : public QMainWindow
{
    Q_OBJECT

public:
    explicit DisplayApplication(QWidget *parent = nullptr);
    ~DisplayApplication();

private:
    Ui::DisplayApplication *ui;
};

#endif // DISPLAYAPPLICATION_H
