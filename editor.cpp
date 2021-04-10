#include "editor.h"
#include "highlighter.h"
#include "customfile.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QTextEdit>
#include <QProcess>
#include <QTimer>
#include <QSettings>

#include <stdexcept>
#include <string>
#include <Windows.h>
#include <WinBase.h>
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
    setupOutputConsole();

    setCentralWidget(editor);
    setWindowTitle(tr("Text Editor IDE"));

    setGeometry(50, 50, 1000, 1000);
}

void Editor::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new QTextEdit;
    editor->setFont(font);

    highlighter = new Highlighter(editor->document(), Highlighter::CPP);
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
    QAction *saveFileAsAction = fileMenu->addAction("&Save File As...");
    fileMenu->addSeparator();
    QAction *closeFileAction = fileMenu->addAction("&Close File");

    newFileAction->setShortcut(QKeySequence::New);
    openFileAction->setShortcut(QKeySequence::Open);
    saveFileAction->setShortcut(QKeySequence::Save);
    saveFileAsAction->setShortcut(QKeySequence::SaveAs);
    closeFileAction->setShortcut(QKeySequence::Close);

    connect(newFileAction, &QAction::triggered, this, &Editor::newFile);
    connect(openFileAction, &QAction::triggered, this, &Editor::openFile);
    connect(saveFileAction, &QAction::triggered, this, &Editor::saveFile);
    connect(saveFileAsAction, &QAction::triggered, this, &Editor::saveFileAs);
    connect(closeFileAction, &QAction::triggered, this, &Editor::closeFile);

    QMenu *runMenu = menuBar()->addMenu("&Run");

    QAction *runFile = runMenu->addAction("&Run");

    runFile->setShortcut(QKeySequence("F5"));

    connect(runFile, &QAction::triggered, this, &Editor::runFile);
}

void Editor::setupOutputConsole()
{
    QToolBar *secondaryToolbar = new QToolBar();
    QAction *outputConsoleAction = secondaryToolbar->addAction("&1 Output");
    outputConsoleAction->setShortcut(QKeySequence("Alt+1"));
    addToolBar(Qt::BottomToolBarArea, secondaryToolbar);
    QDockWidget *outputConsole = new QDockWidget(tr("Output"), this);
    outputConsole->setAllowedAreas(Qt::BottomDockWidgetArea);
    outputConsoleTextArea = new QTextEdit("", this);
    outputConsole->setWidget(outputConsoleTextArea);
    addDockWidget(Qt::BottomDockWidgetArea, outputConsole);

    connect(outputConsoleAction, &QAction::triggered, this, [=](){ outputConsole->isHidden() ? outputConsole->show() : outputConsole->hide(); });
}

QPair<int, QAction*> Editor::findOpenFileAction(int index)
{
    for (QPair<int, QAction*> pair : openFilesActions) {
        if (pair.first == index) {
            return pair;
        }
    }
}

void Editor::runFile()
{
    saveFile();
    CustomFile *file = openFiles.find(currentOpenFileIndex);

    if (CustomFile::getEnding(file->fileName)==".py") {
        QProcess consoleProcess;
        consoleProcess.start("python \"" + file->filePath.toUtf8() + "\"");
        consoleProcess.waitForFinished(-1);
        outputConsoleTextArea->setText(QString::fromUtf8(consoleProcess.readAll()));
    }
}

void Editor::newFile()
{
    if (currentOpenFileIndex != -1) {
        CustomFile *oldFile = openFiles.find(currentOpenFileIndex);
        oldFile->currentText = editor->toPlainText();
    }

    CustomFile newFile = CustomFile(rand() % INT_MAX, NEW_FILE_NAME, NEW_FILE_NAME, "");

    openFiles.append(newFile);

    QPair<int, QAction*> newToolbar;
    newToolbar.first = newFile.index;
    newToolbar.second = toolBar->addAction(newFile.fileName);
    connect(newToolbar.second, &QAction::triggered, this, [=](){ swapOpenFile(newFile.index); });
    openFilesActions.append(newToolbar);
    toolBar->addSeparator();

    currentOpenFileIndex = newFile.index;

    editor->setPlainText(newFile.currentText);
}

void Editor::openFile()
{
    QString fileLocation = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/Users/desktop/debug", tr("Any Files (*.*)"));

    if (CustomFile::getEnding(fileLocation) == ".py") {
        delete highlighter;
        highlighter = new Highlighter(editor->document(), Highlighter::PYTHON);
    }


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

    QPair<int, QAction*> toolbarName;
    toolbarName.first = openedFile.index;
    toolbarName.second = toolBar->addAction(openedFile.fileName);
    openFilesActions.append(toolbarName);
    connect(toolbarName.second, &QAction::triggered, this, [=](){ swapOpenFile(openedFile.index); });
    toolBar->addSeparator();

    file.close();
}

void Editor::saveFile()
{
    CustomFile *fileToSave = openFiles.find(currentOpenFileIndex);
    if (fileToSave->filePath == NEW_FILE_NAME) {
        saveFileAs();
        return;
    }
    QFile file(fileToSave->filePath);
    if (file.open(QFile::Text | QFile::WriteOnly)) {
        QTextStream out(&file);
        out << editor->toPlainText();
    } else {
        QMessageBox errorBox;
        errorBox.setText("This file could not be saved.");
        errorBox.exec();
    }
}

void Editor::saveFileAs()
{
    CustomFile *fileToSave = openFiles.find(currentOpenFileIndex);
    QUrl fileUrl = QFileDialog::getSaveFileUrl(this, tr("Save File As...", ""), tr("All Files (*)"));
    QFile file(QString::fromStdString(fileUrl.toString().toStdString().substr(8)));
    if (file.open(QFile::Text | QFile::WriteOnly)) {
        file.write(editor->toPlainText().toUtf8());
        fileToSave->filePath = fileUrl.toString();
        fileToSave->setFileNameWithFilePath(fileUrl.toString());

        toolBar->removeAction(findOpenFileAction(fileToSave->index).second);
        QPair<int, QAction*> toolbarName;
        toolbarName.first = fileToSave->index;
        toolbarName.second = toolBar->addAction(fileToSave->fileName);
        openFilesActions.append(toolbarName);
        connect(toolbarName.second, &QAction::triggered, this, [=](){ swapOpenFile(fileToSave->index); });
    } else {
        QMessageBox errorBox;
        errorBox.setText("This file could not be saved.");
        errorBox.exec();
    }
}

void Editor::closeFile()
{
    std::string saveText = "Would you like to save \"" + openFiles.find(currentOpenFileIndex)->fileName.toStdString() + "\"?";
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr(saveText.c_str()), tr(saveText.c_str()));
    if (reply == QMessageBox::Yes) {
        saveFile();
    }
    toolBar->removeAction(openFilesActions.find(currentOpenFileIndex));
    editor->clear();
}

void Editor::closeEvent(QCloseEvent *event)
{
    for (QPair<int, QAction*> pair : openFilesActions) {
        currentOpenFileIndex = pair.first;
        closeFile();
    }
    QMainWindow::closeEvent(event);
}
