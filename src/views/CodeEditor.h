#pragma once

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTimer>

namespace nascode {
namespace views {

class CodeCompleter;
class SnippetManager;

/**
 * @brief ST语言语法高亮器
 */
class STSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit STSyntaxHighlighter(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> m_highlightingRules;

    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_dataTypeFormat;
    QTextCharFormat m_commentFormat;
    QTextCharFormat m_stringFormat;
    QTextCharFormat m_numberFormat;
};

/**
 * @brief ST代码编辑器
 */
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget* parent = nullptr);
    ~CodeEditor() override;

    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();

    void setFilePath(const QString& filePath) { m_filePath = filePath; }
    QString filePath() const { return m_filePath; }

    /**
     * @brief 获取代码补全器
     */
    CodeCompleter* completer() const { return m_completer; }

    /**
     * @brief 获取代码片段管理器
     */
    SnippetManager* snippetManager() const { return m_snippetManager; }

    /**
     * @brief 切换注释(选中行或当前行)
     */
    void toggleComment();

    /**
     * @brief 自动格式化选中文本或全部代码
     */
    void formatCode();

    /**
     * @brief 跳转到指定行
     */
    void gotoLine(int line);

    /**
     * @brief 查找匹配的括号
     */
    void findMatchingBracket();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect& rect, int dy);
    void onTextChanged();
    void checkAutoCompletion();

private:
    void highlightMatchingBrackets();
    void autoIndent();
    QString getIndentation(const QString& line) const;
    bool isOpeningBracket(QChar c) const;
    bool isClosingBracket(QChar c) const;
    QChar getMatchingBracket(QChar c) const;

private:
    QWidget* m_lineNumberArea;
    STSyntaxHighlighter* m_highlighter;
    QString m_filePath;
    
    // 智能编辑功能
    CodeCompleter* m_completer;
    SnippetManager* m_snippetManager;
    QTimer* m_completionTimer;
    
    // 括号匹配
    QList<QTextEdit::ExtraSelection> m_bracketSelections;
    
    // 配置
    bool m_autoIndentEnabled;
    bool m_bracketMatchingEnabled;
    bool m_autoCompletionEnabled;
    int m_tabSize;
};

/**
 * @brief 行号区域
 */
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor* editor) : QWidget(editor), m_codeEditor(editor) {}

    QSize sizeHint() const override {
        return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        m_codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor* m_codeEditor;
};

} // namespace views
} // namespace nascode
