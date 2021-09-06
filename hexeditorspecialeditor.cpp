#include "hexeditorspecialeditor.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>

HexEditorSpecialEditor::HexEditorSpecialEditor(QWidget *parent) : QTabWidget(parent)
{
    inspectorTab = new QWidget();
    analysisTab = new QTableWidget(AnalysisTableRow::__SIZE__, 2, this);

    //QVBoxLayout *analysisLayout = new QVBoxLayout();

    //analysisTable = new QTableWidget(10, 2, this);

    analysisTab->setColumnWidth(0, 200);
    analysisTab->setColumnWidth(1, 250);

    analysisTab->setItem(AnalysisTableRow::FILETYPE, 0, new QTableWidgetItem("File Type"));
    analysisTab->setItem(AnalysisTableRow::ZIP_EXTRACT_VERSION, 0, new QTableWidgetItem("Zip Minimum Extract Version"));
    analysisTab->setItem(AnalysisTableRow::GENERAL_BIT_FLAG, 0, new QTableWidgetItem("General Purpose Bit Flag"));
    analysisTab->setItem(AnalysisTableRow::ZIP_COMPRESSION_METHOD, 0, new QTableWidgetItem("Zip Compression Method"));
    analysisTab->setItem(AnalysisTableRow::LAST_MODIFICATION, 0, new QTableWidgetItem("Last Modification"));

    //analysisTab->setLayout(analysisLayout);

    addTab(analysisTab, "Analysis");
    addTab(inspectorTab, "Inspector");
}

void HexEditorSpecialEditor::setData(QByteArray data) {
    this->data = data;
}

bool samelengthcmp(char *a, char *b, int len) {
    for (int i = 0; i < len; i++) {
        if (*a != *b) return false;
        ++a;
        ++b;
    }
    return true;
}

QString longToDOSTimeAndDate(unsigned long time) {
    qDebug() << time;
    QString output = "";
    output.setNum(time, 2);
    qDebug() << output;
    output.setNum(time, 16);
    qDebug() << output;
    return "";
}

void HexEditorSpecialEditor::hideUnusedRows() {
    for (int item = AnalysisTableRow::FILETYPE; item != AnalysisTableRow::__SIZE__; item++) {
        if (analysisTab->item(item, 1) == NULL) {
            analysisTab->hideRow(item);
        }
    }
}

void HexEditorSpecialEditor::analyse() {
    for (int item = AnalysisTableRow::FILETYPE; item != AnalysisTableRow::__SIZE__; item++) {
        analysisTab->setItem(item, 1, NULL);
    }

    if (data.size() >= 4) {
        QDataStream stream(&data, QIODevice::ReadOnly);
        char buffer[4];
        stream.readRawData(buffer, 4);
        if (samelengthcmp(buffer, new char[4]{0x50, 0x4b, 0x03, 0x04}, 4)) {
            analysisTab->setItem(AnalysisTableRow::FILETYPE, 1, new QTableWidgetItem("Zip archive"));

            unsigned char buffer2[2];
            stream.readRawData((char*)buffer2, 2);

            QString outputStr("");
            outputStr.setNum(buffer2[1] << 8 | buffer2[0], 10);
            outputStr.insert(outputStr.length() - 1, '.');

            analysisTab->setItem(AnalysisTableRow::ZIP_EXTRACT_VERSION, 1, new QTableWidgetItem(outputStr));

            stream.readRawData((char*)buffer2, 2);
            outputStr.setNum(buffer2[1] << 8 | buffer2[0], 2);
            outputStr = outputStr.rightJustified(16, '0');
            outputStr.insert(0, 'b');
            outputStr.insert(0, '0');

            analysisTab->setItem(AnalysisTableRow::GENERAL_BIT_FLAG, 1, new QTableWidgetItem(outputStr));

            stream.readRawData((char*)buffer2, 2);
            outputStr = compressionTable[buffer2[1] << 8 | buffer2[0]];

            analysisTab->setItem(AnalysisTableRow::GENERAL_BIT_FLAG, 1, new QTableWidgetItem(outputStr));

            unsigned char buffer4[4];

            stream.readRawData((char*)buffer4, 4);
            outputStr = longToDOSTimeAndDate(buffer4[1] << 24 | buffer4[0] << 16 | buffer4[3] << 8 | buffer4[2]);

            analysisTab->setItem(AnalysisTableRow::LAST_MODIFICATION, 1, new QTableWidgetItem(outputStr));
        } else {
            analysisTab->setItem(AnalysisTableRow::FILETYPE, 1, new QTableWidgetItem("Unknown"));
        }
    }
    hideUnusedRows();
}
