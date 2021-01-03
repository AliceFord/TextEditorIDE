#include "editor.h"
#include "ui_editor.h"
#include "highlighter.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

Editor::Editor(QWidget *parent)
    : QMainWindow(parent)
{
    setupMenuBars();
    setupEditor();

    setCentralWidget(editor);
    setWindowTitle(tr("Syntax Highlighter"));
}

void Editor::setupMenuBars()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openFileAction = fileMenu->addAction("&Open File");
    openFileAction->setShortcut(QKeySequence::Open);
    connect(openFileAction, &QAction::triggered, this, &Editor::openFile);
}

void Editor::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new QTextEdit;
    editor->setFont(font);

    highlighter = new Highlighter(editor->document());

    QFile file("C:/Users/olive/Desktop/Coding/After Da USB/Qt/TextEditorIDE/editor.h");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
    file.close();
}

void Editor::openFile()
{
    QString fileLocation = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", tr("Any Files (*.*)"));
    QFile file(fileLocation);
    if (file.open(QFile::Text | QFile::ReadOnly))
        editor->setPlainText(file.readAll());
    else {
        QMessageBox errorBox;
        errorBox.setText("This file cannot be opened.");
        errorBox.exec();
    }
    file.close();
}
