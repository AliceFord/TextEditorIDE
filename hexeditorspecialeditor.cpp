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
    analysisTab->setItem(AnalysisTableRow::JPEG_EXTRACT_VERSION, 0, new QTableWidgetItem("JPEG Version"));
    analysisTab->setItem(AnalysisTableRow::GENERAL_BIT_FLAG, 0, new QTableWidgetItem("General Purpose Bit Flag"));
    analysisTab->setItem(AnalysisTableRow::ZIP_COMPRESSION_METHOD, 0, new QTableWidgetItem("Zip Compression Method"));
    analysisTab->setItem(AnalysisTableRow::LAST_MODIFICATION, 0, new QTableWidgetItem("Last Modification"));
    analysisTab->setItem(AnalysisTableRow::FILE_NAME, 0, new QTableWidgetItem("Original File Name"));
    analysisTab->setItem(AnalysisTableRow::IMG_WIDTH, 0, new QTableWidgetItem("Image Width"));
    analysisTab->setItem(AnalysisTableRow::IMG_HEIGHT, 0, new QTableWidgetItem("Image Height"));
    analysisTab->setItem(AnalysisTableRow::IMG_BITDEPTH, 0, new QTableWidgetItem("Image Bitdepth"));
    analysisTab->setItem(AnalysisTableRow::IMG_COLOR_TYPE, 0, new QTableWidgetItem("Image Color Type"));
    analysisTab->setItem(AnalysisTableRow::IMG_COMPRESSION, 0, new QTableWidgetItem("Image Compression Method"));
    analysisTab->setItem(AnalysisTableRow::IMG_FILTER_METHOD, 0, new QTableWidgetItem("Image Filter Method"));
    analysisTab->setItem(AnalysisTableRow::IMG_INTERLACE_METHOD, 0, new QTableWidgetItem("Image Interlace Method"));

    //analysisTab->setLayout(analysisLayout);

    addTab(analysisTab, "Analysis");
    addTab(inspectorTab, "Inspector");
}

void HexEditorSpecialEditor::setData(QByteArray data) {
    this->data = data;
}

bool samelengthcmp(unsigned char *a, unsigned char *b, int len) {
    for (int i = 0; i < len; i++) {
        if (*a != *b) return false;
        ++a;
        ++b;
    }
    return true;
}

QString longToDOSTimeAndDate(unsigned long time) { // 10000110111001100101001100100100
    QString output = "";
    output += QString::number((time) & 0b11111).rightJustified(2, '0');
    output += "/";
    output += QString::number((time >> 5) & 0b1111).rightJustified(2, '0');
    output += "/";
    output += QString::number(((time >> 9) & 0b1111111) + 1980);
    output += " ";
    output += QString::number((time >> 27) & 0b11111).rightJustified(2, '0');
    output += ":";
    output += QString::number((time >> 21) & 0b111111).rightJustified(2, '0');
    output += ":";
    output += QString::number(((time >> 16) & 0b11111) << 1).rightJustified(2, '0');
    return output;
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
        QString output("");

        QDataStream stream(&data, QIODevice::ReadOnly);
        unsigned char buffer[256];
        stream.readRawData((char*)buffer, 4);
        if (samelengthcmp(buffer, new unsigned char[4]{0x50, 0x4b, 0x03, 0x04}, 4)) {
            analysisTab->setItem(AnalysisTableRow::FILETYPE, 1, new QTableWidgetItem("Zip archive"));

            stream.readRawData((char*)buffer, 2);

            output.setNum(buffer[1] << 8 | buffer[0], 10);
            output.insert(output.length() - 1, '.');

            analysisTab->setItem(AnalysisTableRow::ZIP_EXTRACT_VERSION, 1, new QTableWidgetItem(output));

            stream.readRawData((char*)buffer, 2);
            output.setNum(buffer[1] << 8 | buffer[0], 2);
            output = output.rightJustified(16, '0');
            output.insert(0, 'b');
            output.insert(0, '0');

            analysisTab->setItem(AnalysisTableRow::GENERAL_BIT_FLAG, 1, new QTableWidgetItem(output));

            stream.readRawData((char*)buffer, 2);
            output = compressionTable[buffer[1] << 8 | buffer[0]];

            analysisTab->setItem(AnalysisTableRow::GENERAL_BIT_FLAG, 1, new QTableWidgetItem(output));

            stream.readRawData((char*)buffer, 4);
            output = longToDOSTimeAndDate(buffer[1] << 24 | buffer[0] << 16 | buffer[3] << 8 | buffer[2]);

            analysisTab->setItem(AnalysisTableRow::LAST_MODIFICATION, 1, new QTableWidgetItem(output));

            stream.readRawData((char*)buffer, 4); // Checksum

            stream.readRawData((char*)buffer, 4); // Compressed Size

            stream.readRawData((char*)buffer, 4); // Uncompressed Size

            stream.readRawData((char*)buffer, 2);
            short fileNameLen = buffer[1] << 8 | buffer[0];

            stream.readRawData((char*)buffer, 2);
            short extraFieldLen = buffer[1] << 8 | buffer[0];

            unsigned char buffer1[1];
            output = "";

            for (int i = 0; i < fileNameLen; i++) {
                stream.readRawData((char*)buffer, 1);
                output += buffer[0];
            }
            analysisTab->setItem(AnalysisTableRow::FILE_NAME, 1, new QTableWidgetItem(output));
        } else if (samelengthcmp(buffer, new unsigned char[4]{0x89, 0x50, 0x4e, 0x47}, 4)) {
            stream.readRawData((char*)buffer, 4);
            if (samelengthcmp(buffer, new unsigned char[4]{0x0d, 0x0a, 0x1a, 0x0a}, 4)) {
                analysisTab->setItem(AnalysisTableRow::FILETYPE, 1, new QTableWidgetItem("PNG Image"));

                stream.readRawData((char*)buffer, 4);
                int length = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];

                stream.readRawData((char*)buffer, 4);
                if (!samelengthcmp(buffer, new unsigned char[4]{0x49, 0x48, 0x44, 0x52}, 4)) goto analysisEnd;

                stream.readRawData((char*)buffer, 4);
                output.setNum(buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);
                analysisTab->setItem(AnalysisTableRow::IMG_WIDTH, 1, new QTableWidgetItem(output));

                stream.readRawData((char*)buffer, 4);
                output.setNum(buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);
                analysisTab->setItem(AnalysisTableRow::IMG_HEIGHT, 1, new QTableWidgetItem(output));

                stream.readRawData((char*)buffer, 1);
                output.setNum(buffer[0]);
                output += "-bit";
                analysisTab->setItem(AnalysisTableRow::IMG_BITDEPTH, 1, new QTableWidgetItem(output));


                stream.readRawData((char*)buffer, 1);
                analysisTab->setItem(AnalysisTableRow::IMG_COLOR_TYPE, 1, new QTableWidgetItem(pngColorTypeTable[buffer[0]]));

                stream.readRawData((char*)buffer, 1);
                output.setNum(buffer[0]);
                analysisTab->setItem(AnalysisTableRow::IMG_COMPRESSION, 1, new QTableWidgetItem(output));

                stream.readRawData((char*)buffer, 1);
                output.setNum(buffer[0]);
                analysisTab->setItem(AnalysisTableRow::IMG_FILTER_METHOD, 1, new QTableWidgetItem(output));

                stream.readRawData((char*)buffer, 1);
                output = (buffer[0] == 0 ? "No Interlace" : "Adam7 Interlace");
                analysisTab->setItem(AnalysisTableRow::IMG_INTERLACE_METHOD, 1, new QTableWidgetItem(output));
            } else {
                analysisTab->setItem(AnalysisTableRow::FILETYPE, 1, new QTableWidgetItem("Unknown"));
            }

        } else if (samelengthcmp(buffer, new unsigned char[4]{0xff, 0xd8, 0xff, 0xe0}, 4)) {
            stream.readRawData((char*)buffer, 7);
            if (samelengthcmp(buffer, new unsigned char[7]{0x00, 0x10, 0x4a, 0x46, 0x49, 0x46, 0x00}, 7)) {
                analysisTab->setItem(AnalysisTableRow::FILETYPE, 1, new QTableWidgetItem("JPEG Image"));

                stream.readRawData((char*)buffer, 1);
                output.setNum(buffer[0]);
                output += ".";
                QString temp("");
                stream.readRawData((char*)buffer, 1);
                temp.setNum(buffer[0]);
                temp = temp.rightJustified(2, '0');
                output += temp;
                analysisTab->setItem(AnalysisTableRow::JPEG_EXTRACT_VERSION, 1, new QTableWidgetItem(output));
            } else {
                analysisTab->setItem(AnalysisTableRow::FILETYPE, 1, new QTableWidgetItem("Unknown"));
            }
        } else {
            analysisTab->setItem(AnalysisTableRow::FILETYPE, 1, new QTableWidgetItem("Unknown"));
        }
    }
analysisEnd:
    hideUnusedRows();
}
