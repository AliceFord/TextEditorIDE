#include "hexeditor.h"
#include "qdebug.h"

HexEditor::HexEditor(QWidget *parent) : QMainWindow(parent)
{
    setGeometry(100, 100, 800, 800);

    editor = new HexeditWidget();
//    QFont f("unexistent");
//    f.setStyleHint(QFont::Monospace);
//    //display->setFont(f);
    setCentralWidget(editor);
}

void HexEditor::setFile(CustomFile *file) {
    this->file = file;
    editor->setData(new QByteArray((file->currentText).toStdString().c_str()));
    //display->setPlainText("yo");
}