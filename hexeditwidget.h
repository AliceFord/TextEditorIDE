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

    QPen blackPen;
    QPen grayPen;
    QPen bluePen;
};

#endif // HEXEDITWIDGET_H
