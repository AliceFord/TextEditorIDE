#include "hexeditorspecialeditor.h"
#include <QDebug>

HexEditorSpecialEditor::HexEditorSpecialEditor(QByteArray *data, QWidget *parent) : QTabWidget(parent)
{
    this->data = data;
    inspectorTab = new QWidget();
    analysisTab = new QWidget();

    addTab(analysisTab, "Analysis");
    addTab(inspectorTab, "Inspector");
}

void HexEditorSpecialEditor::analyse() {
//    QDataStream ds(*data);
//    unsigned char header;
//    ds >> header;
}
