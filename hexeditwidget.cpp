#include "hexeditwidget.h"
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QKeyEvent>
#include <QScrollBar>

HexeditWidget::HexeditWidget()
{
    grabKeyboard();

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    blackPen = *(new QPen(Qt::black));
    grayPen = *(new QPen(Qt::darkGray));
    bluePen = *(new QPen(Qt::blue));
    selector = *(new NibbleSelector());
}

void HexeditWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QFont font = painter.font();
    font.setFamily("Courier New");
    font.setPointSize(11);
    painter.setFont(font);

    QString offsetText = "Offset";
    painter.setPen(bluePen);
    painter.drawText(10, 16 + scrollValue, offsetText);

    for (int i = 0; i < 16; i++) {
        offsetText.setNum(i, 16);
        offsetText = offsetText.rightJustified(2, '0');
        painter.drawText(80 + 28 * (i + 1), 16 + scrollValue, offsetText);
    }

    offsetText = "Decoded Text";
    painter.drawText(570, 16 + scrollValue, offsetText);

    QString current;
    QString offset;
    for (int i = 0; i < std::ceil(data->size() / 16.0); i++) {
        offset.setNum(i * 16, 16);
        offset = offset.rightJustified(8, '0');
        painter.setPen(bluePen);
        painter.drawText(10, 20 + 16 * (i + 1) + scrollValue, offset);
        for (int j = 0; j < 16; j++) {
            if (i * 16 + j >= data->size()) break;
            if (j % 2 == 0) painter.setPen(blackPen);
            else painter.setPen(grayPen);
            current.setNum((unsigned char)data->at(i * 16 + j), 16);
            current = current.rightJustified(2, '0');
            painter.drawText(80 + 28 * (j + 1), 20 + 16 * (i + 1) + scrollValue, current);

            painter.setPen(blackPen);

            current = data->at(i * 16 + j);
            painter.drawText(570 + 10 * j, 20 + 16 * (i + 1) + scrollValue, current);
        }
    }

    // Cursor
    while ((20 + 16 * (selector.byteCounter / 16 + 1) + scrollValue) < 0) {
        selector.byteCounter += 16;
    }

    while ((20 + 16 * (selector.byteCounter / 16 + 1) + scrollValue) > height()) {
        selector.byteCounter -= 16;
    }

    int xpos = 80 + 28 * (selector.byteCounter % 16 + 1);
    xpos += selector.nibbleCounter * 9;
    int ypos = 20 + 16 * (selector.byteCounter / 16 + 1);
    painter.drawLine(xpos, ypos+2 + scrollValue, xpos, ypos-12 + scrollValue);
    painter.drawLine(xpos+1, ypos+2 + scrollValue, xpos+1, ypos-12 + scrollValue);

    xpos = 570 + 10 * (selector.byteCounter % 16);
    ypos = 20 + 16 * (selector.byteCounter / 16 + 1);
    painter.drawLine(xpos, ypos+2 + scrollValue, xpos, ypos-12 + scrollValue);
    painter.drawLine(xpos+1, ypos+2 + scrollValue, xpos+1, ypos-12 + scrollValue);
}

void HexeditWidget::editData(QKeyEvent *event) {
    char keyPressed = event->text().at(0).toLatin1();
    QString intKey(keyPressed);
    char current = data->at(selector.byteCounter);
    undoStack.push(UndoableActionDescriptor(UndoableAction::CHANGE_NIBBLE, selector, data->at(selector.byteCounter)));
    if (selector.nibbleCounter == 0) {
        current = ((intKey.toShort(nullptr, 16) & 0xf) << 4) | (current & 0xf);
    } else {
        current = (((current >> 4) & 0xf) << 4) | (intKey.toShort(nullptr, 16) & 0xf);
    }

    (*data)[selector.byteCounter] = current;

    selector++;
}

void HexeditWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
    case Qt::Key_A:
    case Qt::Key_B:
    case Qt::Key_C:
    case Qt::Key_D:
    case Qt::Key_E:
    case Qt::Key_F:
        editData(event);
        break;
    case Qt::Key_Left:
        selector--;
        break;
    case Qt::Key_Right:
        selector++;
        break;
    case Qt::Key_Up:
        if (selector.byteCounter >= 16) selector.byteCounter -= 16;
        break;
    case Qt::Key_Down:
        if (selector.byteCounter / 16 < data->size() / 16) selector.byteCounter += 16;
        break;
    case Qt::Key_Z:
        if (event->modifiers().testFlag(Qt::KeyboardModifier::ControlModifier)) {
            undo();
        }
        break;
    default:
        break;
    }
    update();
}

void HexeditWidget::wheelEvent(QWheelEvent *event) {
    if (!((long long)event->angleDelta().y() > -scrollValue)) {
        scrollValue += event->angleDelta().y();
    }

    update();
}

void HexeditWidget::setData(QByteArray *data) {
    this->data = data;
    selector = *(new NibbleSelector());
    update();
}

QByteArray HexeditWidget::getData() {
    return *data;
}

QByteArray *HexeditWidget::getDataPtr() {
    return data;
}

void HexeditWidget::undo() {
    UndoableActionDescriptor undoEvent = undoStack.pop();
    if (undoEvent.action == UndoableAction::CHANGE_NIBBLE) {
        (*data)[undoEvent.selector.byteCounter] = undoEvent.prevValue;
        this->selector = undoEvent.selector;
    }
}

