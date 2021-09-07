#ifndef HEXEDITORSPECIALEDITOR_H
#define HEXEDITORSPECIALEDITOR_H

#include <QObject>
#include <QTabWidget>
#include <QTableWidget>

class HexEditorSpecialEditor : public QTabWidget
{
    Q_OBJECT
public:
    explicit HexEditorSpecialEditor(QWidget *parent = nullptr);
    void setData(QByteArray data);
    void analyse();

signals:

private:
    enum AnalysisTableRow {
        FILETYPE,
        ZIP_EXTRACT_VERSION,
        GENERAL_BIT_FLAG,
        ZIP_COMPRESSION_METHOD,
        LAST_MODIFICATION,
        FILE_NAME,
        __SIZE__
    };

    enum FileType {
        ZIP_FILE
    };

    void hideUnusedRows();

    QWidget *inspectorTab;
    QTableWidget *analysisTab;

    QByteArray data;

    QList<QString> compressionTable = {"No Compression", "Shrunk", "Reduced w/compression 1", "Reduced w/compression 2", "Reduced w/compression 3", "Reduced w/compression 4", "Imploded", "Deflated", "Enhanced Deflated", "PKWare DCL Imploded", "Reserved", "Compressed w/BZIP2", "Reserved", "LZMA", "Reserved", "Reserved", "Reserved", "IBM TERSE", "IBM LZ77 z"};
};

#endif // HEXEDITORSPECIALEDITOR_H
