#include "hexeditwidget.h"
#include <QPainter>
#include <QDebug>
#include <cmath>

HexeditWidget::HexeditWidget()
{
    blackPen = *(new QPen(Qt::black));
    grayPen = *(new QPen(Qt::darkGray));
}

void HexeditWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(viewport());
    QFont font = painter.font();
    font.setFamily("Courier New");
    font.setPointSize(11);
    painter.setFont(font);
    QString current;
    QString offset;
    for (int i = 0; i < std::ceil(data->size() / 16.0); i++) {
        offset.setNum(i * 16, 16);
        offset = offset.rightJustified(8, '0');
        painter.setPen(blackPen);
        painter.drawText(10, 16 * (i + 1), offset);
        for (int j = 0; j < 16; j++) {
            if (i * 16 + j >= data->size()) break;
            if (j % 2 == 0) painter.setPen(blackPen);
            else painter.setPen(grayPen);
            current.setNum(data->at(i * 16 + j), 16);
            current = current.rightJustified(2, '0');
            painter.drawText(80 + 28 * (j + 1), 16 * (i + 1), current);
        }
    }
}

void HexeditWidget::setData(QByteArray *data) {
    this->data = data;
    update();
}

