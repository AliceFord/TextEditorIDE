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
    void keyPressEvent(QKeyEvent *event);

private:
    struct NibbleSelector {
        int byteCounter;
        uchar nibbleCounter;

        NibbleSelector& operator++() {
            if (this->nibbleCounter == 0) {
                this->nibbleCounter++;
            } else {
                this->nibbleCounter = 0;
                this->byteCounter++;
            }
            return *this;
        }

        NibbleSelector operator++(int) {
            NibbleSelector temp = *this;
            ++*this;
            return temp;
        }

        NibbleSelector() {
            this->byteCounter = 0;
            this->nibbleCounter = 0;
        }
    };

    QByteArray *data;

    QPen blackPen;
    QPen grayPen;
    QPen bluePen;

    NibbleSelector selector;
};

#endif // HEXEDITWIDGET_H
