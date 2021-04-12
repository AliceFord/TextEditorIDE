#include "findandreplace.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QCheckBox>

FindAndReplaceWindow::FindAndReplaceWindow(Editor *editor, Highlighter *highlighter)
{
    this->highlighter = highlighter;
    this->editor = editor;

    setupLayout();

    setWindowFlags(windowFlags());

    //setCentralWidget(editor);
    setWindowTitle(tr("Find and Replace"));

    setGeometry(400, 400, 500, 300);
}

void FindAndReplaceWindow::setupLayout()
{
    QGroupBox *groupBox = new QGroupBox(tr("Options"));

    QLabel *label = new QLabel(tr("Find what: "));
    lineEdit = new QLineEdit();
    QCheckBox *checkbox = new QCheckBox("&Replace");

    connect(lineEdit, &QLineEdit::editingFinished, this, &FindAndReplaceWindow::dataEntryFinished);
    connect(checkbox, &QCheckBox::clicked, this, &FindAndReplaceWindow::replaceBoxPressed);

    QVBoxLayout *vbox = new QVBoxLayout();

    QHBoxLayout *hbox = new QHBoxLayout();

    hbox->addWidget(label);
    hbox->addWidget(lineEdit);
    hbox->addWidget(checkbox);

    QWidget *mainWidget1 = new QWidget();
    mainWidget1->setLayout(hbox);

    QLabel *label2 = new QLabel(tr("Replace what: "));
    replaceLineEdit = new QLineEdit();
    replaceLineEdit->setText(DEFAULT_REPLACE_TEXT);

    connect(replaceLineEdit, &QLineEdit::editingFinished, this, &FindAndReplaceWindow::replaceEntryFinished);

    QHBoxLayout *hbox2 = new QHBoxLayout();

    hbox2->addWidget(label2);
    hbox2->addWidget(replaceLineEdit);

    mainWidget2 = new QWidget();
    mainWidget2->setLayout(hbox2);
    mainWidget2->setVisible(false);

    vbox->addWidget(mainWidget1);
    vbox->addWidget(mainWidget2);

    groupBox->setLayout(vbox);

    setCentralWidget(groupBox);
}

void FindAndReplaceWindow::replaceBoxPressed(bool checked)
{
    mainWidget2->setVisible(checked);
}

void FindAndReplaceWindow::replaceEntryFinished()
{
    highlighter->setReplaceTerm(replaceLineEdit->text());
}

void FindAndReplaceWindow::dataEntryFinished()
{
    if (previousSearchTerm == lineEdit->text()) {
        QPair<QRegularExpressionMatch, QTextCharFormat> replace = highlighter->cycleSearch();
        if (replace.first.capturedStart() != -1) {
            editor->replaceText(replace.first.capturedStart(), replace.first.capturedLength(), replaceLineEdit->text());
        }
        qDebug() << replace.first.capturedStart();
    } else {
        previousSearchTerm = lineEdit->text();
        QTextDocument *doc = highlighter->getParent();
        int language = highlighter->getLanguage();
        delete highlighter;
        if (replaceLineEdit->text() == DEFAULT_REPLACE_TEXT)
            highlighter = new Highlighter(doc, language, lineEdit->text());
        else
            highlighter = new Highlighter(doc, language, lineEdit->text(), replaceLineEdit->text());
    }
}
