#include "CodeEditor.h"
#include "CodeCompleter.h"
#include <QPainter>
#include <QTextBlock>
#include <QRegularExpression>
#include <QCompleter>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QKeyEvent>
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

void CodeEditor::onTextChanged()
{
    // 文本改变时的处理
}

void CodeEditor::checkAutoCompletion()
{
    QString prefix = m_completer->getCurrentPrefix();
    if (prefix.length() >= 2) {
        m_completer->showCompletion(prefix);
    }
}

void CodeEditor::toggleComment()
{
    QTextCursor cursor = textCursor();
    
    // 获取选中的行范围
    int startLine = cursor.blockNumber();
    int endLine = startLine;
    
    if (cursor.hasSelection()) {
        int start = cursor.selectionStart();
        int end = cursor.selectionEnd();
        cursor.setPosition(start);
        startLine = cursor.blockNumber();
        cursor.setPosition(end);
        endLine = cursor.blockNumber();
    }
    
    cursor.beginEditBlock();
    
    // 检查是否所有行都已注释
    bool allCommented = true;
    QTextBlock block = document()->findBlockByNumber(startLine);
    for (int i = startLine; i <= endLine && block.isValid(); ++i) {
        QString text = block.text().trimmed();
        if (!text.isEmpty() && !text.startsWith("//")) {
            allCommented = false;
            break;
        }
        block = block.next();
    }
    
    // 切换注释
    block = document()->findBlockByNumber(startLine);
    for (int i = startLine; i <= endLine && block.isValid(); ++i) {
        cursor.setPosition(block.position());
        cursor.movePosition(QTextCursor::StartOfBlock);
        
        if (allCommented) {
            // 取消注释
            QString text = block.text();
            int commentPos = text.indexOf("//");
            if (commentPos >= 0) {
                cursor.setPosition(block.position() + commentPos);
                cursor.deleteChar();
                cursor.deleteChar();
            }
        } else {
            // 添加注释
            cursor.insertText("//");
        }
        
        block = block.next();
    }
    
    cursor.endEditBlock();
}

void CodeEditor::formatCode()
{
    // TODO: 实现代码格式化
    spdlog::info("Code formatting not yet implemented");
}

void CodeEditor::gotoLine(int line)
{
    QTextCursor cursor(document()->findBlockByNumber(line - 1));
    setTextCursor(cursor);
    centerCursor();
}

void CodeEditor::findMatchingBracket()
{
    QTextCursor cursor = textCursor();
    QChar c = document()->characterAt(cursor.position());
    
    if (isOpeningBracket(c)) {
        // 向前查找配对括号
        int depth = 1;
        QChar closing = getMatchingBracket(c);
        
        for (int pos = cursor.position() + 1; pos < document()->characterCount(); ++pos) {
            QChar ch = document()->characterAt(pos);
            if (ch == c) depth++;
            else if (ch == closing) {
                depth--;
                if (depth == 0) {
                    cursor.setPosition(pos);
                    setTextCursor(cursor);
                    return;
                }
            }
        }
    } else if (isClosingBracket(c)) {
        // 向后查找配对括号
        int depth = 1;
        QChar opening = getMatchingBracket(c);
        
        for (int pos = cursor.position() - 1; pos >= 0; --pos) {
            QChar ch = document()->characterAt(pos);
            if (ch == c) depth++;
            else if (ch == opening) {
                depth--;
                if (depth == 0) {
                    cursor.setPosition(pos);
                    setTextCursor(cursor);
                    return;
                }
            }
        }
    }
}

void CodeEditor::highlightMatchingBrackets()
{
    m_bracketSelections.clear();
    
    QTextCursor cursor = textCursor();
    int pos = cursor.position();
    
    // 检查光标前后的字符
    QChar charBefore = (pos > 0) ? document()->characterAt(pos - 1) : QChar();
    QChar charAfter = document()->characterAt(pos);
    
    auto highlightPair = [this](int pos1, int pos2) {
        QTextEdit::ExtraSelection selection1;
        selection1.format.setBackground(QColor(Qt::yellow).lighter(160));
        QTextCursor cursor1 = textCursor();
        cursor1.setPosition(pos1);
        cursor1.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        selection1.cursor = cursor1;
        m_bracketSelections.append(selection1);
        
        QTextEdit::ExtraSelection selection2;
        selection2.format.setBackground(QColor(Qt::yellow).lighter(160));
        QTextCursor cursor2 = textCursor();
        cursor2.setPosition(pos2);
        cursor2.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        selection2.cursor = cursor2;
        m_bracketSelections.append(selection2);
    };
    
    // 查找匹配的括号
    auto findMatch = [this](int startPos, QChar open, QChar close, bool forward) -> int {
        int depth = 1;
        int step = forward ? 1 : -1;
        int endPos = forward ? document()->characterCount() : 0;
        
        for (int p = startPos + step; forward ? (p < endPos) : (p >= endPos); p += step) {
            QChar ch = document()->characterAt(p);
            if (ch == open) depth++;
            else if (ch == close) {
                depth--;
                if (depth == 0) return p;
            }
        }
        return -1;
    };
    
    if (isOpeningBracket(charBefore)) {
        QChar closing = getMatchingBracket(charBefore);
        int match = findMatch(pos - 1, charBefore, closing, true);
        if (match >= 0) {
            highlightPair(pos - 1, match);
        }
    } else if (isClosingBracket(charBefore)) {
        QChar opening = getMatchingBracket(charBefore);
        int match = findMatch(pos - 1, charBefore, opening, false);
        if (match >= 0) {
            highlightPair(match, pos - 1);
        }
    }
    
    if (isOpeningBracket(charAfter)) {
        QChar closing = getMatchingBracket(charAfter);
        int match = findMatch(pos, charAfter, closing, true);
        if (match >= 0) {
            highlightPair(pos, match);
        }
    } else if (isClosingBracket(charAfter)) {
        QChar opening = getMatchingBracket(charAfter);
        int match = findMatch(pos, charAfter, opening, false);
        if (match >= 0) {
            highlightPair(match, pos);
        }
    }
    
    // 应用高亮
    QList<QTextEdit::ExtraSelection> selections = extraSelections();
    selections.append(m_bracketSelections);
    setExtraSelections(selections);
}

void CodeEditor::autoIndent()
{
    QTextCursor cursor = textCursor();
    QTextBlock previousBlock = cursor.block().previous();
    
    if (previousBlock.isValid()) {
        QString previousLine = previousBlock.text();
        QString indent = getIndentation(previousLine);
        
        // 如果上一行以THEN, DO, STRUCT等结尾,增加缩进
        QString trimmed = previousLine.trimmed();
        if (trimmed.endsWith("THEN") || trimmed.endsWith("DO") ||
            trimmed.endsWith("STRUCT") || trimmed.endsWith("VAR") ||
            trimmed.endsWith("VAR_INPUT") || trimmed.endsWith("VAR_OUTPUT")) {
            indent += QString(m_tabSize, ' ');
        }
        
        cursor.insertText(indent);
    }
}

QString CodeEditor::getIndentation(const QString& line) const
{
    QString indent;
    for (QChar c : line) {
        if (c == ' ' || c == '\t') {
            indent += c;
        } else {
            break;
        }
    }
    return indent;
}

bool CodeEditor::isOpeningBracket(QChar c) const
{
    return c == '(' || c == '[' || c == '{';
}

bool CodeEditor::isClosingBracket(QChar c) const
{
    return c == ')' || c == ']' || c == '}';
}

QChar CodeEditor::getMatchingBracket(QChar c) const
{
    if (c == '(') return ')';
    if (c == ')') return '(';
    if (c == '[') return ']';
    if (c == ']') return '[';
    if (c == '{') return '}';
    if (c == '}') return '{';
    return QChar();
}

void CodeEditor::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::keyPressEvent(QKeyEvent* event)
{
    // 处理补全器
    if (m_completer->completer()->popup()->isVisible()) {
        // 补全弹窗可见时的按键处理
        switch (event->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            event->ignore();
            return;
        default:
            break;
        }
    }

    // Tab键处理 - 尝试展开代码片段或插入空格
    if (event->key() == Qt::Key_Tab && event->modifiers() == Qt::NoModifier) {
        QString prefix = m_completer->getCurrentPrefix();
        if (!prefix.isEmpty() && m_snippetManager->tryExpandSnippet(prefix)) {
            return;  // 成功展开片段
        }
        // 否则插入空格
        insertPlainText(QString(m_tabSize, ' '));
        return;
    }

    // Ctrl+Space - 手动触发补全
    if (event->key() == Qt::Key_Space && event->modifiers() == Qt::ControlModifier) {
        QString prefix = m_completer->getCurrentPrefix();
        m_completer->showCompletion(prefix);
        return;
    }

    // Ctrl+/ - 切换注释
    if (event->key() == Qt::Key_Slash && event->modifiers() == Qt::ControlModifier) {
        toggleComment();
        return;
    }

    // Ctrl+] - 增加缩进
    if (event->key() == Qt::Key_BracketRight && event->modifiers() == Qt::ControlModifier) {
        QTextCursor cursor = textCursor();
        cursor.insertText(QString(m_tabSize, ' '));
        return;
    }

    // Ctrl+[ - 减少缩进
    if (event->key() == Qt::Key_BracketLeft && event->modifiers() == Qt::ControlModifier) {
        QTextCursor cursor = textCursor();
        QString line = cursor.block().text();
        if (line.startsWith(QString(m_tabSize, ' '))) {
            cursor.movePosition(QTextCursor::StartOfBlock);
            for (int i = 0; i < m_tabSize; ++i) {
                cursor.deleteChar();
            }
        }
        return;
    }

    // Enter键 - 自动缩进
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        QPlainTextEdit::keyPressEvent(event);
        if (m_autoIndentEnabled) {
            autoIndent();
        }
        return;
    }

    // 自动配对括号
    QChar c = event->text().isEmpty() ? QChar() : event->text().at(0);
    if (c == '(' || c == '[' || c == '{') {
        QTextCursor cursor = textCursor();
        cursor.insertText(event->text());
        
        // 插入配对的右括号
        QChar closing;
        if (c == '(') closing = ')';
        else if (c == '[') closing = ']';
        else if (c == '{') closing = '}';
        
        cursor.insertText(QString(closing));
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        return;
    }

    // 默认处理
    QPlainTextEdit::keyPressEvent(event);

    // 触发自动补全
    if (m_autoCompletionEnabled && event->text().length() > 0 && 
        event->text().at(0).isLetterOrNumber()) {
        m_completionTimer->start();
    }
}

void CodeEditor::paintEvent(QPaintEvent* event)
{
    QPlainTextEdit::paintEvent(event);
    
    // 高亮匹配的括号
    if (m_bracketMatchingEnabled) {
        highlightMatchingBrackets();
    }
}

void CodeEditor::focusInEvent(QFocusEvent* event)
{
    if (m_completer) {
        m_completer->completer()->setWidget(this);
    }
    QPlainTextEdit::focusInEvent(event);
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

} // namespace views
} // namespace nascode
