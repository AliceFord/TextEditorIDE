#include "hexeditwidget.h"
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QKeyEvent>

HexeditWidget::HexeditWidget()
{
    blackPen = *(new QPen(Qt::black));
    grayPen = *(new QPen(Qt::darkGray));
    bluePen = *(new QPen(Qt::blue));
    selector = *(new NibbleSelector());
}

void HexeditWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(viewport());
    QFont font = painter.font();
    font.setFamily("Courier New");
    font.setPointSize(11);
    painter.setFont(font);

    QString offsetText = "Offset";
    painter.setPen(bluePen);
    painter.drawText(10, 16, offsetText);

    for (int i = 0; i < 16; i++) {
        offsetText.setNum(i, 16);
        offsetText = offsetText.rightJustified(2, '0');
        painter.drawText(80 + 28 * (i + 1), 16, offsetText);
    }

    offsetText = "Decoded Text";
    painter.drawText(570, 16, offsetText);

    QString current;
    QString offset;
    for (int i = 0; i < std::ceil(data->size() / 16.0); i++) {
        offset.setNum(i * 16, 16);
        offset = offset.rightJustified(8, '0');
        painter.setPen(bluePen);
        painter.drawText(10, 20 + 16 * (i + 1), offset);
        for (int j = 0; j < 16; j++) {
            if (i * 16 + j >= data->size()) break;
            if (j % 2 == 0) painter.setPen(blackPen);
            else painter.setPen(grayPen);
            current.setNum(data->at(i * 16 + j), 16);
            current = current.rightJustified(2, '0');
            painter.drawText(80 + 28 * (j + 1), 20 + 16 * (i + 1), current);

            painter.setPen(blackPen);

            current = data->at(i * 16 + j);
            painter.drawText(570 + 10 * j, 20 + 16 * (i + 1), current);
        }
    }
}

void HexeditWidget::keyPressEvent(QKeyEvent *event) {
    char keyPressed = event->text().at(0).toLatin1();
    if ((keyPressed >= '0' && keyPressed <= '9') || (keyPressed >= 'a' && keyPressed <= 'f')) {
        QString intKey(keyPressed);
        char current = data->at(selector.byteCounter);
        if (selector.nibbleCounter == 0) {
            current = ((intKey.toShort(nullptr, 16) & 0xf) << 4) | (current & 0xf);
        } else {
            current = (((current >> 4) & 0xf) << 4) | (intKey.toShort(nullptr, 16) & 0xf);
        }

        (*data)[selector.byteCounter] = current;

        selector++;

        update();
    }
}

void HexeditWidget::setData(QByteArray *data) {
    this->data = data;
    update();
}

