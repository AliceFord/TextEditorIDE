#include "editor.h"
#include "highlighter.h"
#include "customfile.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Editor::Editor(QWidget *parent)
    : QMainWindow(parent)
{
    srand(time(NULL));
    setupMenuBars();
    setupEditor();
    setupFileBar();

    setCentralWidget(editor);
    setWindowTitle(tr("Syntax Highlighter"));
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

void Editor::swapOpenFile(int fileToChangeToIndex)
{
    openFiles.find(currentOpenFileIndex)->currentText = editor->toPlainText();
    editor->setPlainText(openFiles.find(fileToChangeToIndex)->currentText);
    currentOpenFileIndex = openFiles.find(fileToChangeToIndex)->index;
}

void Editor::setupFileBar()
{
    toolBar = addToolBar("Files");
}

void Editor::setupMenuBars()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *newFileAction = fileMenu->addAction("&New File");
    QAction *openFileAction = fileMenu->addAction("&Open File");
    QAction *saveFileAction = fileMenu->addAction("&Save File");

    newFileAction->setShortcut(QKeySequence::New);
    openFileAction->setShortcut(QKeySequence::Open);
    saveFileAction->setShortcut(QKeySequence::Save);

    connect(newFileAction, &QAction::triggered, this, &Editor::newFile);
    connect(openFileAction, &QAction::triggered, this, &Editor::openFile);
    connect(saveFileAction, &QAction::triggered, this, &Editor::saveFile);
}

void Editor::newFile()
{
    editor->setPlainText("");
    // TODO
}

void Editor::openFile()
{
    QString fileLocation = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/Users/desktop/debug", tr("Any Files (*.*)"));
    QFile file(fileLocation);
    if (file.open(QFile::Text | QFile::ReadOnly))
        editor->setPlainText(file.readAll());
    else {
        QMessageBox errorBox;
        errorBox.setText("This file could not be opened.");
        errorBox.exec();
    }
    CustomFile openedFile = CustomFile(rand() % INT_MAX, fileLocation, editor->toPlainText());
    openFiles.append(openedFile);

    currentOpenFileIndex = openedFile.index;

    QAction *toolbarName = toolBar->addAction(openedFile.fileName);
    connect(toolbarName, &QAction::triggered, this, [=](){ swapOpenFile(openedFile.index);qDebug() << openFiles.find(currentOpenFileIndex)->currentText; }); //swapOpenFile(openFiles.find(currentOpenFileIndex), openedFile);
    toolBar->addSeparator();

    file.close();
}

void Editor::saveFile()
{
    QFile file(openFiles.find(currentOpenFileIndex)->filePath);
    if (file.open(QFile::Text | QFile::WriteOnly)) {
        QTextStream out(&file);
        out << editor->toPlainText();
    } else {
        QMessageBox errorBox;
        errorBox.setText("This file could not be saved.");
        errorBox.exec();
    }
}
