#include "CodeCompleter.h"
#include "CodeEditor.h"
#include <QAbstractItemView>
#include <QScrollBar>
#include <QTextCursor>
#include <QTextDocument>
#include <spdlog/spdlog.h>

namespace nascode {
namespace views {

// CodeCompleter实现
CodeCompleter::CodeCompleter(CodeEditor* editor)
    : QObject(editor)
    , m_editor(editor)
    , m_completer(new QCompleter(this))
    , m_model(new QStringListModel(this))
{
    setupDefaultCompletions();
    
    m_completer->setModel(m_model);
    m_completer->setWidget(m_editor);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setWrapAround(false);
}

CodeCompleter::~CodeCompleter()
{
}

void CodeCompleter::setupDefaultCompletions()
{
    // ST语言关键字
    QStringList keywords = {
        // 程序结构
        "PROGRAM", "END_PROGRAM", "FUNCTION", "END_FUNCTION",
        "FUNCTION_BLOCK", "END_FUNCTION_BLOCK",
        
        // 变量声明
        "VAR", "END_VAR", "VAR_INPUT", "VAR_OUTPUT", "VAR_IN_OUT",
        "VAR_TEMP", "VAR_EXTERNAL", "VAR_GLOBAL", "VAR_ACCESS",
        "CONSTANT", "RETAIN", "NON_RETAIN",
        
        // 数据类型
        "BOOL", "BYTE", "WORD", "DWORD", "LWORD",
        "SINT", "INT", "DINT", "LINT",
        "USINT", "UINT", "UDINT", "ULINT",
        "REAL", "LREAL",
        "TIME", "DATE", "TOD", "DT", "DATE_AND_TIME", "TIME_OF_DAY",
        "STRING", "WSTRING",
        "TYPE", "END_TYPE", "STRUCT", "END_STRUCT",
        "ARRAY", "OF",
        
        // 控制流
        "IF", "THEN", "ELSE", "ELSIF", "END_IF",
        "CASE", "OF", "END_CASE",
        "FOR", "TO", "BY", "DO", "END_FOR",
        "WHILE", "END_WHILE",
        "REPEAT", "UNTIL", "END_REPEAT",
        "EXIT", "RETURN",
        
        // 逻辑运算
        "AND", "OR", "XOR", "NOT", "MOD",
        
        // 常量
        "TRUE", "FALSE", "NULL",
        
        // 其他
        "AT", "REF", "REF_TO"
    };

    // 标准函数
    QStringList standardFunctions = {
        // 数学函数
        "ABS", "SQRT", "LN", "LOG", "EXP",
        "SIN", "COS", "TAN", "ASIN", "ACOS", "ATAN",
        "EXPT",
        
        // 位操作
        "SHL", "SHR", "ROL", "ROR",
        
        // 选择函数
        "SEL", "MAX", "MIN", "LIMIT", "MUX",
        
        // 比较
        "GT", "GE", "EQ", "LE", "LT", "NE",
        
        // 类型转换
        "BOOL_TO_INT", "INT_TO_REAL", "REAL_TO_INT",
        "DINT_TO_REAL", "REAL_TO_DINT",
        "INT_TO_STRING", "REAL_TO_STRING",
        
        // 字符串函数
        "LEN", "LEFT", "RIGHT", "MID", "CONCAT",
        "INSERT", "DELETE", "REPLACE", "FIND",
        
        // 时间函数
        "ADD_TIME", "SUB_TIME", "MUL_TIME", "DIV_TIME"
    };

    m_defaultCompletions = keywords + standardFunctions;
    m_model->setStringList(m_defaultCompletions);
}

void CodeCompleter::showCompletion(const QString& prefix)
{
    if (prefix.isEmpty() || prefix.length() < 2) {
        m_completer->popup()->hide();
        return;
    }

    if (prefix != m_completer->completionPrefix()) {
        m_completer->setCompletionPrefix(prefix);
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
    }

    if (m_completer->completionCount() == 0) {
        m_completer->popup()->hide();
        return;
    }

    // 计算弹出位置
    QTextCursor cursor = m_editor->textCursor();
    cursor.movePosition(QTextCursor::StartOfWord);
    QRect rect = m_editor->cursorRect(cursor);
    rect.setWidth(m_completer->popup()->sizeHintForColumn(0)
                  + m_completer->popup()->verticalScrollBar()->sizeHint().width());
    
    m_completer->complete(rect);
}

void CodeCompleter::insertCompletion(const QString& completion)
{
    QTextCursor cursor = m_editor->textCursor();
    
    // 删除已输入的前缀
    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 
                       m_completer->completionPrefix().length());
    cursor.removeSelectedText();
    
    // 插入补全文本
    cursor.insertText(completion);
    m_editor->setTextCursor(cursor);
}

QString CodeCompleter::getCurrentPrefix() const
{
    QTextCursor cursor = m_editor->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    return cursor.selectedText();
}

void CodeCompleter::updateCompletions(const QStringList& customItems)
{
    QStringList completions = m_defaultCompletions;
    
    // 添加自定义项(如变量名、函数名等)
    if (!customItems.isEmpty()) {
        completions.append(customItems);
        completions.removeDuplicates();
        completions.sort(Qt::CaseInsensitive);
    }
    
    m_model->setStringList(completions);
}

// SnippetManager实现
SnippetManager::SnippetManager(CodeEditor* editor)
    : QObject(editor)
    , m_editor(editor)
{
    setupDefaultSnippets();
}

SnippetManager::~SnippetManager()
{
}

void SnippetManager::setupDefaultSnippets()
{
    // PROGRAM片段
    addSnippet(CodeSnippet(
        "prog",
        "Program template",
        "PROGRAM ProgramName\nVAR\n\t\nEND_VAR\n\n\t\nEND_PROGRAM",
        20  // 光标位置在ProgramName后
    ));

    // FUNCTION片段
    addSnippet(CodeSnippet(
        "func",
        "Function template",
        "FUNCTION FunctionName : INT\nVAR_INPUT\n\t\nEND_VAR\nVAR\n\t\nEND_VAR\n\n\t\nEND_FUNCTION",
        17
    ));

    // FUNCTION_BLOCK片段
    addSnippet(CodeSnippet(
        "fb",
        "Function Block template",
        "FUNCTION_BLOCK BlockName\nVAR_INPUT\n\t\nEND_VAR\nVAR_OUTPUT\n\t\nEND_VAR\nVAR\n\t\nEND_VAR\n\n\t\nEND_FUNCTION_BLOCK",
        23
    ));

    // IF语句
    addSnippet(CodeSnippet(
        "if",
        "If statement",
        "IF condition THEN\n\t\nEND_IF",
        3
    ));

    // IF-ELSE语句
    addSnippet(CodeSnippet(
        "ife",
        "If-Else statement",
        "IF condition THEN\n\t\nELSE\n\t\nEND_IF",
        3
    ));

    // FOR循环
    addSnippet(CodeSnippet(
        "for",
        "For loop",
        "FOR i := 0 TO 10 BY 1 DO\n\t\nEND_FOR",
        4
    ));

    // WHILE循环
    addSnippet(CodeSnippet(
        "while",
        "While loop",
        "WHILE condition DO\n\t\nEND_WHILE",
        6
    ));

    // CASE语句
    addSnippet(CodeSnippet(
        "case",
        "Case statement",
        "CASE variable OF\n\t0:\n\t\t\n\tELSE\n\t\t\nEND_CASE",
        5
    ));

    // VAR块
    addSnippet(CodeSnippet(
        "var",
        "Variable declaration block",
        "VAR\n\t\nEND_VAR",
        4
    ));

    // STRUCT
    addSnippet(CodeSnippet(
        "struct",
        "Structure type",
        "TYPE StructName :\nSTRUCT\n\t\nEND_STRUCT\nEND_TYPE",
        5
    ));
}

void SnippetManager::addSnippet(const CodeSnippet& snippet)
{
    m_snippets[snippet.trigger] = snippet;
}

bool SnippetManager::tryExpandSnippet(const QString& trigger)
{
    if (!m_snippets.contains(trigger)) {
        return false;
    }

    const CodeSnippet& snippet = m_snippets[trigger];
    QTextCursor cursor = m_editor->textCursor();

    // 删除触发词
    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, trigger.length());
    cursor.removeSelectedText();

    // 插入代码片段
    int startPos = cursor.position();
    cursor.insertText(snippet.code);
    
    // 移动光标到指定位置
    cursor.setPosition(startPos + snippet.cursorOffset);
    m_editor->setTextCursor(cursor);

    spdlog::debug("Expanded snippet: {}", trigger.toStdString());
    return true;
}

QStringList SnippetManager::getSnippetTriggers() const
{
    return m_snippets.keys();
}

} // namespace views
} // namespace nascode
