#include "CodeEditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QRegularExpression>
#include <spdlog/spdlog.h>

namespace nascode {
namespace views {

// STSyntaxHighlighter实现
STSyntaxHighlighter::STSyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // 关键字
    m_keywordFormat.setForeground(Qt::blue);
    m_keywordFormat.setFontWeight(QFont::Bold);
    
    QStringList keywordPatterns = {
        "\\bPROGRAM\\b", "\\bEND_PROGRAM\\b", "\\bFUNCTION\\b", "\\bEND_FUNCTION\\b",
        "\\bFUNCTION_BLOCK\\b", "\\bEND_FUNCTION_BLOCK\\b",
        "\\bVAR\\b", "\\bEND_VAR\\b", "\\bVAR_INPUT\\b", "\\bVAR_OUTPUT\\b",
        "\\bVAR_IN_OUT\\b", "\\bVAR_TEMP\\b", "\\bVAR_EXTERNAL\\b", "\\bVAR_GLOBAL\\b",
        "\\bIF\\b", "\\bTHEN\\b", "\\bELSE\\b", "\\bELSIF\\b", "\\bEND_IF\\b",
        "\\bCASE\\b", "\\bOF\\b", "\\bEND_CASE\\b",
        "\\bFOR\\b", "\\bTO\\b", "\\bBY\\b", "\\bDO\\b", "\\bEND_FOR\\b",
        "\\bWHILE\\b", "\\bEND_WHILE\\b",
        "\\bREPEAT\\b", "\\bUNTIL\\b", "\\bEND_REPEAT\\b",
        "\\bRETURN\\b", "\\bEXIT\\b",
        "\\bTYPE\\b", "\\bEND_TYPE\\b", "\\bSTRUCT\\b", "\\bEND_STRUCT\\b",
        "\\bARRAY\\b", "\\bOF\\b",
        "\\bAND\\b", "\\bOR\\b", "\\bXOR\\b", "\\bNOT\\b", "\\bMOD\\b"
    };

    for (const QString& pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = m_keywordFormat;
        m_highlightingRules.append(rule);
    }

    // 数据类型
    m_dataTypeFormat.setForeground(Qt::darkCyan);
    m_dataTypeFormat.setFontWeight(QFont::Bold);
    
    QStringList dataTypePatterns = {
        "\\bBOOL\\b", "\\bBYTE\\b", "\\bWORD\\b", "\\bDWORD\\b", "\\bLWORD\\b",
        "\\bSINT\\b", "\\bINT\\b", "\\bDINT\\b", "\\bLINT\\b",
        "\\bUSINT\\b", "\\bUINT\\b", "\\bUDINT\\b", "\\bULINT\\b",
        "\\bREAL\\b", "\\bLREAL\\b",
        "\\bTIME\\b", "\\bDATE\\b", "\\bTOD\\b", "\\bDT\\b",
        "\\bSTRING\\b", "\\bWSTRING\\b"
    };

    for (const QString& pattern : dataTypePatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = m_dataTypeFormat;
        m_highlightingRules.append(rule);
    }

    // 数字
    m_numberFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\\b[0-9]+\\.?[0-9]*\\b");
    rule.format = m_numberFormat;
    m_highlightingRules.append(rule);

    // 字符串
    m_stringFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegularExpression("'[^']*'");
    rule.format = m_stringFormat;
    m_highlightingRules.append(rule);

    // 注释
    m_commentFormat.setForeground(Qt::darkGray);
    m_commentFormat.setFontItalic(true);
}

void STSyntaxHighlighter::highlightBlock(const QString& text)
{
    for (const HighlightingRule& rule : m_highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // 处理注释 (* ... *) 和 // ...
    QRegularExpression commentStartExpression("\\(\\*");
    QRegularExpression commentEndExpression("\\*\\)");
    QRegularExpression singleLineComment("//[^\n]*");

    // 单行注释
    QRegularExpressionMatchIterator matchIterator = singleLineComment.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), m_commentFormat);
    }

    // 多行注释
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
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, m_commentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

// CodeEditor实现
CodeEditor::CodeEditor(QWidget* parent)
    : QPlainTextEdit(parent)
{
    m_lineNumberArea = new LineNumberArea(this);
    m_highlighter = new STSyntaxHighlighter(document());

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    // 设置字体
    QFont font("Consolas", 10);
    font.setStyleHint(QFont::Monospace);
    setFont(font);

    // 设置Tab宽度为4个空格
    setTabStopDistance(fontMetrics().horizontalAdvance(' ') * 4);
}

CodeEditor::~CodeEditor()
{
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect& rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::yellow).lighter(160);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                           Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::keyPressEvent(QKeyEvent* event)
{
    // TODO: 实现自动缩进、代码补全等功能
    QPlainTextEdit::keyPressEvent(event);
}

} // namespace views
} // namespace nascode
