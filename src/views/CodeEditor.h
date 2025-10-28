#pragma once

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

namespace nascode {
namespace views {

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

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect& rect, int dy);

private:
    QWidget* m_lineNumberArea;
    STSyntaxHighlighter* m_highlighter;
    QString m_filePath;
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
