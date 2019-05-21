#ifndef MAPDISPLAY_H
#define MAPDISPLAY_H

#include <QWidget>
#include <QtGui>

namespace Ui {
class MapDisplay;
}

class MapDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit MapDisplay(QWidget *parent = nullptr);
    ~MapDisplay();
protected:
    void paintEvent (QPaintEvent *event);

signals:
public slots:

private:
    Ui::MapDisplay *ui;
};

#endif // MAPDISPLAY_H
