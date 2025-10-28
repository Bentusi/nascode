#!/bin/bash

# NasCode 国际化功能完整性检查

echo "================================================"
echo "  NasCode i18n 功能完整性检查"
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

check_exec() {
    if [ -x "$1" ]; then
        echo "✓ $1 (可执行)"
        ((PASS++))
    else
        echo "✗ $1 (不可执行)"
        ((FAIL++))
    fi
}

echo "1. 核心源文件..."
check_file "src/core/I18nManager.h"
check_file "src/core/I18nManager.cpp"

echo ""
echo "2. 翻译源文件..."
check_file "translations/nascode_zh_CN.ts"
check_file "translations/nascode_en_US.ts"

echo ""
echo "3. 编译后的翻译..."
check_file "build/translations/nascode_zh_CN.qm"
check_file "build/translations/nascode_en_US.qm"

echo ""
echo "4. 文档文件..."
check_file "docs/I18N_GUIDE.md"
check_file "docs/I18N_IMPLEMENTATION.md"
check_file "translations/README.md"
check_file "I18N_COMPLETION_SUMMARY.md"

echo ""
echo "5. 工具脚本..."
check_exec "update_translations.sh"
check_exec "demo_i18n.sh"

echo ""
echo "6. 构建配置..."
check_file "CMakeLists.txt"
check_file "src/core/CMakeLists.txt"

echo ""
echo "7. 主程序..."
check_file "build/NasCode"

echo ""
echo "================================================"
echo "  检查结果"
echo "================================================"
echo "通过: $PASS 项"
echo "失败: $FAIL 项"
echo ""

if [ $FAIL -eq 0 ]; then
    echo "✓ 所有组件完整!"
    echo ""
    echo "i18n功能已就绪,可以使用:"
    echo "  - ./build/NasCode (运行应用)"
    echo "  - ./demo_i18n.sh (查看功能演示)"
    echo "  - ./update_translations.sh (更新翻译)"
    echo ""
    exit 0
else
    echo "✗ 有 $FAIL 个组件缺失!"
    echo "请检查构建过程"
    echo ""
    exit 1
fi
