#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include "customfile.h"
#include "hexeditwidget.h"
#include "editor.h"
#include "hexeditorspecialeditor.h"

class HexEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit HexEditor(Editor *parentEditor, QWidget *parent = nullptr);

    void setFile(CustomFile *file);

protected:
    void closeEvent(QCloseEvent *event);

private:
    CustomFile *file;
    HexeditWidget *editor;
    Editor *parentEditor;

    HexEditorSpecialEditor *specialEditorsWidget;
    QDockWidget *specialEditors;
};

#endif // HEXEDITOR_H
