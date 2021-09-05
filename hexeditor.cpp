#include "hexeditor.h"
#include "qdebug.h"
#include <QMenuBar>
#include <QDockWidget>

HexEditor::HexEditor(Editor *parentEditor, QWidget *parent) : QMainWindow(parent)
{
    this->parentEditor = parentEditor;

    setGeometry(100, 100, 1200, 800);

    editor = new HexeditWidget();


    specialEditors = new QDockWidget(tr("Special Editors"), this);
    specialEditors->setMinimumWidth(400);
    specialEditors->setAllowedAreas(Qt::RightDockWidgetArea);
    specialEditorsWidget = new HexEditorSpecialEditor();
    specialEditors->setWidget(specialEditorsWidget);
    addDockWidget(Qt::RightDockWidgetArea, specialEditors);

    setCentralWidget(editor);
}

void HexEditor::setFile(CustomFile *file) {
    this->file = file;
    editor->setData(&file->currentText);
    //display->setPlainText("yo");
}

void HexEditor::closeEvent(QCloseEvent *event) {
    QByteArray data = editor->getData();
    parentEditor->setTextFromHex(data);
}
