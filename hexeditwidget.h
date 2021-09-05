#ifndef HEXEDITWIDGET_H
#define HEXEDITWIDGET_H

#include <QAbstractScrollArea>
#include <QObject>
#include <QPen>
#include <QStack>

class HexeditWidget : public QWidget
{
    Q_OBJECT
public:
    HexeditWidget();
    void setData(QByteArray *data);
    QByteArray getData();
    QByteArray *getDataPtr();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);


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

        NibbleSelector& operator--() {
            if (this->nibbleCounter == 1) {
                this->nibbleCounter--;
            } else {
                this->nibbleCounter = 1;
                this->byteCounter--;
            }
            return *this;
        }

        NibbleSelector operator--(int) {
            NibbleSelector temp = *this;
            --*this;
            return temp;
        }

        NibbleSelector() {
            this->byteCounter = 0;
            this->nibbleCounter = 0;
        }
    };

    enum UndoableAction {
        CHANGE_NIBBLE
    };

    struct UndoableActionDescriptor {
        UndoableAction action;
        NibbleSelector selector;
        char prevValue;

        UndoableActionDescriptor(UndoableAction action, NibbleSelector selector, char prevValue) {
            this->action = action;
            this->selector = selector;
            this->prevValue = prevValue;
        }

        UndoableActionDescriptor() {

        }
    };

    void undo();
    void editData(QKeyEvent *event);

    QByteArray *data;

    QPen blackPen;
    QPen grayPen;
    QPen bluePen;

    NibbleSelector selector;
    QStack<UndoableActionDescriptor> undoStack;

    long long scrollValue = 0;
};

#endif // HEXEDITWIDGET_H
