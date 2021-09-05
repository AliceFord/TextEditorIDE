#ifndef HEXEDITORSPECIALEDITOR_H
#define HEXEDITORSPECIALEDITOR_H

#include <QObject>
#include <QTabWidget>

class HexEditorSpecialEditor : public QTabWidget
{
    Q_OBJECT
public:
    explicit HexEditorSpecialEditor(QByteArray *data, QWidget *parent = nullptr);
    void analyse();

signals:

private:
     QWidget *inspectorTab;
     QWidget *analysisTab;

     QByteArray *data;
};

#endif // HEXEDITORSPECIALEDITOR_H
