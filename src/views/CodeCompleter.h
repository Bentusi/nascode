#pragma once

#include <QObject>
#include <QCompleter>
#include <QStringListModel>
#include <QTextCursor>

namespace nascode {
namespace views {

class CodeEditor;

/**
 * @brief ST语言代码自动补全器
 */
class CodeCompleter : public QObject
{
    Q_OBJECT

public:
    explicit CodeCompleter(CodeEditor* editor);
    ~CodeCompleter() override;

    /**
     * @brief 显示补全建议
     */
    void showCompletion(const QString& prefix);

    /**
     * @brief 插入选中的补全项
     */
    void insertCompletion(const QString& completion);

    /**
     * @brief 获取当前光标处的前缀
     */
    QString getCurrentPrefix() const;

    /**
     * @brief 更新补全列表(根据上下文)
     */
    void updateCompletions(const QStringList& customItems = QStringList());

    /**
     * @brief 获取QCompleter对象
     */
    QCompleter* completer() const { return m_completer; }

private:
    void setupDefaultCompletions();

private:
    CodeEditor* m_editor;
    QCompleter* m_completer;
    QStringListModel* m_model;
    QStringList m_defaultCompletions;
};

/**
 * @brief ST语言代码片段管理器
 */
class CodeSnippet
{
public:
    QString trigger;      // 触发关键字
    QString description;  // 描述
    QString code;         // 代码模板
    int cursorOffset;     // 插入后光标偏移

    CodeSnippet() : cursorOffset(0) {}
    CodeSnippet(const QString& t, const QString& d, const QString& c, int offset = 0)
        : trigger(t), description(d), code(c), cursorOffset(offset) {}
};

/**
 * @brief 代码片段管理器
 */
class SnippetManager : public QObject
{
    Q_OBJECT

public:
    explicit SnippetManager(CodeEditor* editor);
    ~SnippetManager() override;

    /**
     * @brief 添加代码片段
     */
    void addSnippet(const CodeSnippet& snippet);

    /**
     * @brief 尝试展开代码片段
     * @return 如果成功展开返回true
     */
    bool tryExpandSnippet(const QString& trigger);

    /**
     * @brief 获取所有片段的触发词
     */
    QStringList getSnippetTriggers() const;

private:
    void setupDefaultSnippets();

private:
    CodeEditor* m_editor;
    QMap<QString, CodeSnippet> m_snippets;
};

} // namespace views
} // namespace nascode
