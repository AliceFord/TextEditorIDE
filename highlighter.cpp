#include "highlighter.h"

#include <QColor>
#include <QDebug>

Highlighter::Highlighter(QTextDocument *parent, int language, QString searchTerm, QString replaceTerm)
    : QSyntaxHighlighter(parent)
{
    this->replaceTerm = replaceTerm;
    this->parent = parent;
    this->language = language;
    this->searchTerm = searchTerm;

    switch (language) {
    case CPP:
        setHighlightingRulesCPP();
        break;
    case PYTHON:
        setHighlightingRulesPython();
        break;
    }

    HighlightingRule rule;

    searchTermFormat.setBackground(QColor(155, 255, 155));
    rule.pattern = QRegularExpression(QStringLiteral("%1").arg(searchTerm));
    rule.format = searchTermFormat;
    highlightingRules.append(rule);
}

QPair<QRegularExpressionMatch, QTextCharFormat> Highlighter::cycleSearch(bool cycle)
{
    HighlightingRule rule;
    currentSearchTermFormat.setBackground(Qt::yellow);
    rule.pattern = QRegularExpression(QStringLiteral("%1").arg(searchTerm));
    rule.format = currentSearchTermFormat;

    counter = 0;
    bool hasFound = false;
    QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(this->parent->toPlainText());
    while (matchIterator.hasNext()) {
        if (currentFoundItem == -1) {
            currentFoundItem = 1;
        }
        if (currentFoundItem <= ++counter) {
            hasFound = true;
            if (!cycle)
                currentFoundItem = counter+1;

            QRegularExpressionMatch match = matchIterator.next();
            foundItem.first = match;
            foundItem.second = rule.format;
            break;
        } else {
            if (cycle) {
                if (currentFoundItem <= counter+1 && replaceTerm != DEFAULT_REPLACE_TEXT) {
                    currentFoundItem--;
                    counter--;
                    HighlightingRule rule2;
                    currentSearchTermFormat.setBackground(Qt::blue);
                    rule2.pattern = QRegularExpression(QStringLiteral("%1").arg(searchTerm));
                    rule2.format = currentSearchTermFormat;

                    QRegularExpressionMatch match = matchIterator.next();
                    replaceItem.first = match;
                    replaceItem.second = rule2.format;
                } else {
                    QRegularExpressionMatch dump = matchIterator.next();
                }
            } else {
                matchIterator.next();
            }
        }
    }
    if (!hasFound){
        currentFoundItem = -1;
    }

    rehighlight();

    return replaceItem;
}

void Highlighter::setHighlightingRulesPython()
{
    HighlightingRule rule;
    keywordFormat.setForeground(QColor(167, 29, 93));
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] {
        QStringLiteral("\\bFalse\\b"),
        QStringLiteral("\\bNone\\b"),
        QStringLiteral("\\bTrue\\b"),
        QStringLiteral("\\band\\b"),
        QStringLiteral("\\bas\\b"),
        QStringLiteral("\\bassert\\b"),
        QStringLiteral("\\basync\\b"),
        QStringLiteral("\\bawait\\b"),
        QStringLiteral("\\bbreak\\b"),
        QStringLiteral("\\bclass\\b"),
        QStringLiteral("\\bcontinue\\b"),
        QStringLiteral("\\bdef\\b"),
        QStringLiteral("\\bdel\\b"),
        QStringLiteral("\\belif\\b"),
        QStringLiteral("\\belse\\b"),
        QStringLiteral("\\bexcept\\b"),
        QStringLiteral("\\bfinally\\b"),
        QStringLiteral("\\bfor\\b"),
        QStringLiteral("\\bfrom\\b"),
        QStringLiteral("\\bglobal\\b"),
        QStringLiteral("\\bif\\b"),
        QStringLiteral("\\bimport\\b"),
        QStringLiteral("\\bin\\b"),
        QStringLiteral("\\bis\\b"),
        QStringLiteral("\\blambda\\b"),
        QStringLiteral("\\bnonlocal\\b"),
        QStringLiteral("\\bnot\\b"),
        QStringLiteral("\\bor\\b"),
        QStringLiteral("\\bpass\\b"),
        QStringLiteral("\\braise\\b"),
        QStringLiteral("\\breturn\\b"),
        QStringLiteral("\\btry\\b"),
        QStringLiteral("\\bwhile\\b"),
        QStringLiteral("\\bwith\\b"),
        QStringLiteral("\\byield\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setForeground(QColor(0, 134, 179));
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);

    commentStartExpression = QRegularExpression(QStringLiteral("\"\"\""));
    commentEndExpression = QRegularExpression(QStringLiteral("\"\"\""));
}

void Highlighter::setHighlightingRulesCPP()
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] {
        QStringLiteral("\\bchar\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bdouble\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
        QStringLiteral("\\bfriend\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"),
        QStringLiteral("\\blong\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
        QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
        QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsignals\\b"), QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstruct\\b"),
        QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvirtual\\b"),
        QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bbool\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    if (currentFoundItem != -1) {
        setFormat(foundItem.first.capturedStart(), foundItem.first.capturedLength(), foundItem.second);
    }
//    if (replaceTerm != DEFAULT_REPLACE_TEXT && currentFoundItem > 0) {

//        setFormat(replaceItem.first.capturedStart(), replaceItem.first.capturedLength(), replaceItem.second);
//    }


    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
