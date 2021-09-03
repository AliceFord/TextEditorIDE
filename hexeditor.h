#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include "customfile.h"
#include "hexeditwidget.h"

class HexEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit HexEditor(QWidget *parent = nullptr);

    void setFile(CustomFile *file);

signals:

private:
    CustomFile *file;
    HexeditWidget *editor;
};

#endif // HEXEDITOR_H
