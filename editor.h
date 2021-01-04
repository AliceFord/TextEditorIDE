#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QTextEdit>

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
    void setupFileBar();

    void swapOpenFile(int fileToChangeToIndex);

private:
    void setupEditor();
    void setupMenuBars();

    QTextEdit *editor;
    Highlighter *highlighter;
    CustomVector<CustomFile> openFiles;
    QToolBar *toolBar;
    int currentOpenFileIndex=0;
};
#endif // EDITOR_H
