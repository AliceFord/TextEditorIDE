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
    Highlighter(QTextDocument *parent = 0, int language = 0);
    enum LANGUAGE {
        CPP,
        PYTHON
    };

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
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // HIGHLIGHTER_H
