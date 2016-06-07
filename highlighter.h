#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H


#include <QBrush>
#include <QSyntaxHighlighter>


class Highlighter: public QSyntaxHighlighter
{
public:
    Highlighter(QTextDocument *parent);
    bool isEnabled() const;
    void setEnabled(bool enabled);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat *format;
    };
    QList<HighlightingRule> highlightingRules;

    QBrush brush;
    QTextCharFormat keywordFormat;
    // QTextCharFormat imageFormat;
    // QTextCharFormat linkFormat;
    QTextCharFormat boldText;
    QTextCharFormat italicText;
    QTextCharFormat mdTitle;
    QTextCharFormat codeArea;
    // QTextCharFormat *boldText;
    QTextCharFormat linkFormat;
    bool enabled;

};

#endif // HIGHLIGHTER_H
