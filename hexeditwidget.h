#ifndef HEXEDITWIDGET_H
#define HEXEDITWIDGET_H

#include <QAbstractScrollArea>
#include <QObject>
#include <QPen>

class HexeditWidget : public QAbstractScrollArea
{
    Q_OBJECT
public:
    HexeditWidget();
    void setData(QByteArray *data);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QByteArray *data;
    char nibbleToHex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    QPen blackPen;
    QPen grayPen;
};

#endif // HEXEDITWIDGET_H
