#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QTextEdit>

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
    void openFile();

private:
    void setupEditor();
    void setupMenuBars();

    QTextEdit *editor;
    Highlighter *highlighter;
};
#endif // EDITOR_H
