#include <QTextDocument>
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *document) :
    QSyntaxHighlighter(document),
    enabled(false)
{
    brush = QBrush(Qt::gray);

    keywordFormat.setForeground(QColor("#6c71c4"));
    keywordFormat.setFontWeight(QFont::Bold);

    imageFormat.setForeground(QColor("#cf009a"));

    linkFormat.setForeground(QColor("#4e279a"));

    HighlightingRule rule;

    QString htmlTagRegExp = "<(/?)(%1)[^>]*(/?)>";
    QStringList keywords;
    keywords << htmlTagRegExp.arg("html")
             << "<head>" << "</head>"
             << htmlTagRegExp.arg("link")
             << htmlTagRegExp.arg("script")
             << "<body>" << "</body>"
             << "<title>" << "</title>"
             << "<b>" << "</b>"
             << htmlTagRegExp.arg("p")
             << "<i>" << "</i>"
             << "<u>" << "</u>"
             << "<sup>" << "</sup>"
             << "<sub>" << "</sub>"
             << htmlTagRegExp.arg("h1")
             << htmlTagRegExp.arg("h2")
             << htmlTagRegExp.arg("h3")
             << htmlTagRegExp.arg("h4")
             << htmlTagRegExp.arg("h5")
             << htmlTagRegExp.arg("h6")
             << htmlTagRegExp.arg("br")
             << htmlTagRegExp.arg("hr")
             << "<small>" << "</small>"
             << "<big>" << "</big>"
             << "<strong>" << "</strong>"
             << "<em>" << "</em>"
             << "<center>" << "</center>"
             << "<nobr>" << "</nobr>"
             << "<blockquote>" << "</blockquote>"
             << "<pre>" << "</pre>"
             << "<code>" << "</code>"
             << "<li>" << "</li>"
             << "<ul>" << "</ul>"
             << "<ol>" << "</ol>"
             << "<dl>" << "</dl>"
             << "<table>" << "</table>"
             << "<thead>" << "</thead>"
             << "<tbody>" << "</tbody>"
             << htmlTagRegExp.arg("th")
             << htmlTagRegExp.arg("td")
             << htmlTagRegExp.arg("tr")
             << "<strike>" << "</strike>"
             << "<del>" << "</del>";

    foreach(QString keyword, keywords) {
        rule.pattern = QRegExp(keyword);
        rule.format = &keywordFormat;
        highlightingRules.append(rule);
    }

    rule.pattern = QRegExp(htmlTagRegExp.arg("img"));
    rule.format = &imageFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp(htmlTagRegExp.arg("a"));
    rule.format = &linkFormat;
    highlightingRules.append(rule);

    mdTitle.setFontWeight(QFont::Bold);
    // mdTitle.setFontPointSize(22);
    // mdTitle.setForeground(Qt::black);
    rule.pattern = QRegExp("#\\b[A-Za-z0-9_]+\\b");
    rule.format = &mdTitle;

    // codeArea.setFontWeight(QFont::Bold);
    codeArea.setBackground(brush);
    rule.pattern = QRegExp("\\s\\s\\s\\s.*");
    rule.format = &codeArea;

    highlightingRules.append(rule);
}

bool Highlighter::isEnabled() const
{
    return enabled;
}

void Highlighter::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

void Highlighter::highlightBlock(const QString &text)
{
    // TODO
    if (true) {
        foreach(HighlightingRule rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = text.indexOf(expression);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, *(rule.format));
                index = text.indexOf(expression, index + length);
            }
        }
        setCurrentBlockState(0);
    }
}
