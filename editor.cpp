#include "editor.h"
#include "highlighter.h"
#include "customfile.h"
#include "findandreplace.h"
#include "hexeditor.h"

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
    setupDocks();

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

    connect(editor, &QTextEdit::textChanged, this, &Editor::editorTextChanged);

    highlighter = new Highlighter(editor->document(), Highlighter::CPP);
}

void Editor::swapOpenFile(int fileToChangeToIndex)
{
    openFiles.find(currentOpenFileIndex)->currentText = QByteArray(editor->toPlainText().toStdString().c_str());
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

    QAction *newFileAction = fileMenu->addAction(tr("&New File"));
    QAction *openFileAction = fileMenu->addAction(tr("&Open File"));
    QAction *saveFileAction = fileMenu->addAction(tr("&Save File"));
    QAction *saveFileAsAction = fileMenu->addAction(tr("&Save File As..."));
    fileMenu->addSeparator();
    QAction *closeFileAction = fileMenu->addAction(tr("&Close File"));

    newFileAction->setShortcut(QKeySequence::New);
    openFileAction->setShortcut(QKeySequence::Open);
    saveFileAction->setShortcut(QKeySequence::Save);
    saveFileAsAction->setShortcut(QKeySequence::SaveAs);
    //closeFileAction->setShortcut(QKeySequence::Close);

    connect(newFileAction, &QAction::triggered, this, &Editor::newFile);
    connect(openFileAction, &QAction::triggered, this, &Editor::openFile);
    connect(saveFileAction, &QAction::triggered, this, &Editor::saveFile);
    connect(saveFileAsAction, &QAction::triggered, this, &Editor::saveFileAs);
    connect(closeFileAction, &QAction::triggered, this, &Editor::closeFile);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction *findAndReplaceAction = editMenu->addAction(tr("&Find and Replace"));

    findAndReplaceAction->setShortcut(QKeySequence::Find);

    connect(findAndReplaceAction, &QAction::triggered, this, &Editor::findAndReplace);

    QMenu *runMenu = menuBar()->addMenu("&Run");

    QAction *runFile = runMenu->addAction("&Run");
    QAction *liveHTMLView = runMenu->addAction("&Live HTML View");

    runFile->setShortcut(QKeySequence("F5"));
    liveHTMLView->setShortcut(QKeySequence("Ctrl+M"));

    connect(runFile, &QAction::triggered, this, &Editor::runFile);
    connect(liveHTMLView, &QAction::triggered, this, &Editor::toggleLiveHTMLView);
}

void Editor::findAndReplace()
{
    FindAndReplaceWindow *window = new FindAndReplaceWindow(this, highlighter);
    window->show();
}

void Editor::setupDocks()
{
    QAction *outputConsoleAction = secondaryToolbar->addAction("&1 Output");
    outputConsoleAction->setShortcut(QKeySequence("Alt+1"));
    addToolBar(Qt::BottomToolBarArea, secondaryToolbar);
    QDockWidget *outputConsole = new QDockWidget(tr("Output"), this);
    outputConsole->setAllowedAreas(Qt::BottomDockWidgetArea);
    outputConsoleTextArea = new QTextEdit("", this);
    outputConsole->setWidget(outputConsoleTextArea);
    addDockWidget(Qt::BottomDockWidgetArea, outputConsole);

    connect(outputConsoleAction, &QAction::triggered, this, [=](){ outputConsole->isHidden() ? outputConsole->show() : outputConsole->hide(); });

    QAction *liveHTMLViewAction = secondaryToolbar->addAction("&2 Live HTML View");
    liveHTMLViewAction->setShortcut(QKeySequence("Alt+2"));
    liveHTMLView = new QDockWidget(tr("Live HTML View"), this);
    liveHTMLView->setAllowedAreas(Qt::RightDockWidgetArea);
    liveHTMLViewWidget = new QTextBrowser(this);
    liveHTMLViewWidget->setHtml("");
    liveHTMLView->setWidget(liveHTMLViewWidget);
    liveHTMLView->hide();
    addDockWidget(Qt::RightDockWidgetArea, liveHTMLView);

    connect(liveHTMLViewAction, &QAction::triggered, this, [=](){ liveHTMLView->isHidden() ? liveHTMLView->show() : liveHTMLView->hide(); });

    QAction *hexEditorAction = secondaryToolbar->addAction("&3 Hex Editor");
    hexEditorAction->setShortcut(QKeySequence("Alt+3"));

    HexEditor *editor = new HexEditor(this);
    connect(hexEditorAction, &QAction::triggered, editor, [=](){ editor->setFile(openFiles.find(currentOpenFileIndex)); editor->show(); });
}

QPair<int, QAction*> Editor::findOpenFileAction(int index, bool second)
{
    for (QPair<int, QAction*> pair : openFilesActions) {
        if (pair.first == index) {
            if (!second)
                return pair;
            else
                second = false;
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

void Editor::toggleLiveHTMLView()
{
    liveHTMLView->isHidden() ? liveHTMLView->show() : liveHTMLView->hide();
}

void Editor::editorTextChanged()
{
    if (currentOpenFileIndex != -1) {
        CustomFile *oldFile = openFiles.find(currentOpenFileIndex);
        if (oldFile->isSaved) {
            oldFile->isSaved = false;
            renameFile(oldFile, oldFile->fileName + "*");

        }
    }

    if (!liveHTMLView->isHidden()) {
        liveHTMLViewWidget->setHtml(editor->toPlainText());
    }
}

void Editor::renameFile(CustomFile *file, QString newName)
{
    QPair<int, QAction*> before = findOpenFileAction(file->index);
    QPair<int, QAction*> toolbarName;
    toolbarName.first = rand() % INT_MAX;
    toolbarName.second = new QAction(newName);
    toolBar->insertAction(before.second, toolbarName.second);

    CustomFile newFile = *file;
    newFile.index = toolbarName.first;

    connect(toolbarName.second, &QAction::triggered, this, [=](){ swapOpenFile(toolbarName.first); });

    QPair<int, QAction*> deletedAction = openFilesActions.removeByNumber(before.first);
    openFiles.removeByNumber(before.first);

    openFilesActions.append(toolbarName);
    openFiles.append(newFile);
    toolBar->removeAction(deletedAction.second);
    currentOpenFileIndex = toolbarName.first;
}

void Editor::newFile()
{
    if (currentOpenFileIndex != -1) {
        CustomFile *oldFile = openFiles.find(currentOpenFileIndex);
        oldFile->currentText = QByteArray(editor->toPlainText().toStdString().c_str());
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
    QString fileLocation = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\Users\\olive\\Desktop\\debug", tr("Any Files (*.*)"));

    if (CustomFile::getEnding(fileLocation) == ".py") {
        delete highlighter;
        highlighter = new Highlighter(editor->document(), Highlighter::PYTHON);
    }

    QFile file(fileLocation);
    QByteArray fileData(file.size(), '\0');
    if (file.open(QFile::ReadOnly)) { // QFile::Text
        QDataStream fileStream(&file);
        fileStream.readRawData(fileData.data(), file.size());
        editor->setPlainText(fileData);
        file.close();
    } else {
        QMessageBox errorBox;
        errorBox.setText("This file could not be opened.");
        errorBox.exec();
    }
    CustomFile openedFile = CustomFile(rand() % INT_MAX, fileLocation, fileData);
    openFiles.append(openedFile);

    currentOpenFileIndex = openedFile.index;

    QPair<int, QAction*> toolbarName;
    toolbarName.first = openedFile.index;
    toolbarName.second = toolBar->addAction(openedFile.fileName);
    openFilesActions.append(toolbarName);
    connect(toolbarName.second, &QAction::triggered, this, [=](){ swapOpenFile(openedFile.index); });
    toolBar->addSeparator();
}

void Editor::saveFile()
{
    CustomFile *fileToSave = openFiles.find(currentOpenFileIndex);
    if (!fileToSave->isSaved) {
        if (fileToSave->filePath == NEW_FILE_NAME) {
            saveFileAs();
            return;
        }
        fileToSave->updateFilenameFromFilepath();
        QFile file(fileToSave->filePath);
        if (file.open(QFile::WriteOnly)) { // QFile::Text
            QTextStream out(&file);
            out << editor->toPlainText();
            fileToSave->isSaved = true;
            renameFile(fileToSave, fileToSave->fileName);
        } else {
            QMessageBox errorBox;
            errorBox.setText("This file could not be saved.\n" + file.errorString());
            errorBox.exec();
        }
    }
}

void Editor::saveFileAs()
{
    CustomFile *fileToSave = openFiles.find(currentOpenFileIndex);
    QUrl fileUrl = QFileDialog::getSaveFileUrl(this, tr("Save File As...", ""), tr("All Files (*)"));
    QFile file(QString::fromStdString(fileUrl.toString().toStdString().substr(8)));
    if (file.open(QFile::WriteOnly)) { // QFile::Text
        file.write(editor->toPlainText().toUtf8());
        fileToSave->filePath = fileUrl.toLocalFile();
        qDebug() << CustomFile::getFileNameWithFilePath(fileUrl.toLocalFile());
        renameFile(fileToSave, CustomFile::getFileNameWithFilePath(fileUrl.toLocalFile()));
    } else {
        QMessageBox errorBox;
        errorBox.setText("This file could not be saved.");
        errorBox.exec();
    }
}

void Editor::closeFile()
{
    if (!openFiles.find(currentOpenFileIndex)->isSaved) {
        std::string saveText = "Would you like to save \"" + openFiles.find(currentOpenFileIndex)->fileName.toStdString() + "\"?";
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr(saveText.c_str()), tr(saveText.c_str()));
        if (reply == QMessageBox::Yes) {
            saveFile();
        }
    }
    toolBar->removeAction(openFilesActions.find(currentOpenFileIndex));
    toolBar->update();
    editor->clear();
    openFilesActions.removeByNumber(openFiles.find(currentOpenFileIndex)->index);
    openFiles.removeByNumber(openFiles.find(currentOpenFileIndex)->index);
}

void Editor::closeEvent(QCloseEvent *event)
{
    for (QPair<int, QAction*> pair : openFilesActions) {
        currentOpenFileIndex = pair.first;
        closeFile();
    }
    QMainWindow::closeEvent(event);
}

void Editor::replaceText(int startIndex, int length, QString replaceText)
{
    QString text = editor->toPlainText();
    editor->setText(text.replace(startIndex, length, replaceText));
}

void Editor::setTextFromHex(QByteArray hex) {
    editor->setText(hex);
}
