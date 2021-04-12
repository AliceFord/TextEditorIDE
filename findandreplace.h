#ifndef FINDANDREPLACE_H
#define FINDANDREPLACE_H

#include "highlighter.h"
#include "editor.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QString>

class FindAndReplaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    FindAndReplaceWindow(Editor *editor, Highlighter *highlighter);

private slots:
    void dataEntryFinished();
    void replaceEntryFinished();
    void replaceBoxPressed(bool checked);

private:
    void setupLayout();

    Highlighter *highlighter;
    QLineEdit *lineEdit;
    QLineEdit *replaceLineEdit;
    QString previousSearchTerm = "";

    QWidget *mainWidget2;
    Editor *editor;

    const QString DEFAULT_REPLACE_TEXT = "nein";
};

#endif // FINDANDREPLACE_H
