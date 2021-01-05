#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPair>
#include <QProcess>

#include "customvector.h"
#include "customfile.h"
#include "highlighter.h"

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0);

public slots:
    //void about();
    //void newFile();
    //void openFile(const QString &path = QString());

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

    void runFile();

    void setupFileBar();

    void swapOpenFile(int fileToChangeToIndex);

private:
    void setupEditor();
    void setupMenuBars();

    void setupOutputConsole();

    QPair<int, QAction*> findOpenFileAction(int index);

    const QString NEW_FILE_NAME = "*untitled";
    QTextEdit *editor;
    Highlighter *highlighter;
    CustomVector<CustomFile> openFiles;
    QVector<QPair<int, QAction*>> openFilesActions;
    QToolBar *toolBar;

    QTextEdit *outputConsoleTextArea;

    QProcess consoleProcess;

    int currentOpenFileIndex=-1;
};
#endif // EDITOR_H
