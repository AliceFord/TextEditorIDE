#ifndef HEXEDITORSPECIALEDITOR_H
#define HEXEDITORSPECIALEDITOR_H

#include <QObject>
#include <QTabWidget>

class HexEditorSpecialEditor : public QTabWidget
{
    Q_OBJECT
public:
    explicit HexEditorSpecialEditor(QWidget *parent = nullptr);

signals:

private:
     QWidget *inspectorTab;
     QWidget *analysisTab;
};

#endif // HEXEDITORSPECIALEDITOR_H
