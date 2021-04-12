#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QVector>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0, int language = 0, QString searchTerm = "", QString replaceTerm = "nein");
    enum Language {
        CPP,
        PYTHON
    };

    QTextDocument *getParent() {
        return parent;
    }

    int getLanguage() {
        return language;
    }

    void setReplaceTerm(QString replaceTerm) {
        this->replaceTerm = replaceTerm;
    }

    QPair<QRegularExpressionMatch, QTextCharFormat> cycleSearch(bool cycle);

protected:
    void highlightBlock(const QString &text) override;

private:
    void setHighlightingRulesCPP();
    void setHighlightingRulesPython();
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QTextDocument *parent;
    int language;
    QString searchTerm;
    QString replaceTerm;

    int currentFoundItem = -1;
    int counter = 0;

    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat searchTermFormat;
    QTextCharFormat currentSearchTermFormat;

    QPair<QRegularExpressionMatch, QTextCharFormat> foundItem;
    QPair<QRegularExpressionMatch, QTextCharFormat> replaceItem;

    const QString DEFAULT_REPLACE_TEXT = "nein";
};

#endif // HIGHLIGHTER_H
