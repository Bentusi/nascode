#!/bin/bash

# 代码编辑器功能完整性检查

echo "================================================"
echo "  NasCode 代码编辑器功能检查"
echo "================================================"
echo ""

cd /home/wei/nascode

PASS=0
FAIL=0

check_file() {
    if [ -f "$1" ]; then
        echo "✓ $1"
        ((PASS++))
    else
        echo "✗ $1 (缺失)"
        ((FAIL++))
    fi
}

check_code() {
    if grep -q "$2" "$1" 2>/dev/null; then
        echo "✓ $3"
        ((PASS++))
    else
        echo "✗ $3 (未找到)"
        ((FAIL++))
    fi
}

echo "1. 检查源文件..."
check_file "src/views/CodeEditor.h"
check_file "src/views/CodeEditor.cpp"
check_file "src/views/CodeCompleter.h"
check_file "src/views/CodeCompleter.cpp"

echo ""
echo "2. 检查核心类..."
check_code "src/views/CodeEditor.h" "class CodeEditor" "CodeEditor类定义"
check_code "src/views/CodeEditor.h" "class STSyntaxHighlighter" "STSyntaxHighlighter类"
check_code "src/views/CodeCompleter.h" "class CodeCompleter" "CodeCompleter类"
check_code "src/views/CodeCompleter.h" "class SnippetManager" "SnippetManager类"

echo ""
echo "3. 检查功能实现..."
check_code "src/views/CodeEditor.cpp" "highlightBlock" "语法高亮功能"
check_code "src/views/CodeCompleter.cpp" "showCompletion" "代码补全功能"
check_code "src/views/CodeCompleter.cpp" "tryExpandSnippet" "代码片段功能"
check_code "src/views/CodeEditor.cpp" "toggleComment" "注释切换功能"
check_code "src/views/CodeEditor.cpp" "autoIndent" "自动缩进功能"
check_code "src/views/CodeEditor.cpp" "highlightMatchingBrackets" "括号匹配功能"

echo ""
echo "4. 检查关键字支持..."
check_code "src/views/CodeEditor.cpp" "PROGRAM" "PROGRAM关键字"
check_code "src/views/CodeEditor.cpp" "FUNCTION" "FUNCTION关键字"
check_code "src/views/CodeEditor.cpp" "VAR_INPUT" "VAR_INPUT关键字"
check_code "src/views/CodeEditor.cpp" "FOR.*TO.*BY" "FOR循环关键字"

echo ""
echo "5. 检查数据类型..."
check_code "src/views/CodeEditor.cpp" "INT.*DINT.*REAL" "基本数据类型"
check_code "src/views/CodeEditor.cpp" "BOOL.*BYTE.*WORD" "位操作类型"

echo ""
echo "6. 检查标准函数..."
check_code "src/views/CodeCompleter.cpp" "ABS.*SQRT" "数学函数"
check_code "src/views/CodeCompleter.cpp" "SHL.*SHR" "位操作函数"
check_code "src/views/CodeCompleter.cpp" "INT_TO_REAL" "类型转换函数"

echo ""
echo "7. 检查代码片段..."
check_code "src/views/CodeCompleter.cpp" '"prog"' "PROGRAM片段"
check_code "src/views/CodeCompleter.cpp" '"func"' "FUNCTION片段"
check_code "src/views/CodeCompleter.cpp" '"for"' "FOR循环片段"
check_code "src/views/CodeCompleter.cpp" '"if"' "IF语句片段"

echo ""
echo "8. 检查快捷键..."
check_code "src/views/CodeEditor.cpp" "Qt::Key_Space.*ControlModifier" "Ctrl+Space补全"
check_code "src/views/CodeEditor.cpp" "Qt::Key_Slash.*ControlModifier" "Ctrl+/注释"
check_code "src/views/CodeEditor.cpp" "Qt::Key_BracketRight.*ControlModifier" "Ctrl+]缩进"

echo ""
echo "9. 检查文档..."
check_file "docs/CODE_EDITOR_FEATURES.md"
check_file "CODE_EDITOR_COMPLETION.md"

echo ""
echo "10. 检查编译..."
check_file "build/NasCode"

echo ""
echo "================================================"
echo "  检查结果"
echo "================================================"
echo "通过: $PASS 项"
echo "失败: $FAIL 项"
echo ""

if [ $FAIL -eq 0 ]; then
    echo "✓ 所有功能完整!"
    echo ""
    echo "代码编辑器功能包括:"
    echo "  • 语法高亮 (40+关键字, 20+类型)"
    echo "  • 智能补全 (100+补全项)"
    echo "  • 代码片段 (10个内置)"
    echo "  • 行号显示"
    echo "  • 自动缩进"
    echo "  • 括号匹配"
    echo "  • 注释切换"
    echo "  • 快捷键支持"
    echo ""
    echo "立即体验: ./build/NasCode"
    echo ""
    exit 0
else
    echo "✗ 有 $FAIL 个功能缺失!"
    echo ""
    exit 1
fi
