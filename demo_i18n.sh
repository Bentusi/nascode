#!/bin/bash

# NasCode i18n 功能演示脚本

echo "================================================"
echo "  NasCode 国际化 (i18n) 功能演示"
echo "================================================"
echo ""

cd /home/wei/nascode

echo "1. 检查翻译文件..."
echo "   中文翻译: translations/nascode_zh_CN.ts"
echo "   英文翻译: translations/nascode_en_US.ts"
ls -lh translations/*.ts

echo ""
echo "2. 检查编译后的翻译文件..."
echo "   编译后的文件位于: build/translations/"
ls -lh build/translations/*.qm

echo ""
echo "3. 翻译统计..."
echo ""
echo "   中文翻译:"
grep -c "<translation>" translations/nascode_zh_CN.ts | xargs echo "   - 翻译条目数:"

echo ""
echo "   英文翻译:"
grep -c "<translation>" translations/nascode_en_US.ts | xargs echo "   - 翻译条目数:"

echo ""
echo "4. 核心功能展示..."
echo ""
echo "   ✓ I18nManager - 单例国际化管理器"
echo "   ✓ 自动系统语言检测"
echo "   ✓ 持久化语言设置 (QSettings)"
echo "   ✓ 动态语言切换"
echo "   ✓ 中英文双语支持"
echo ""

echo "5. 主窗口翻译项..."
echo ""
echo "   菜单栏:"
echo "   - 文件 (File): 新建/打开/保存/关闭/退出"
echo "   - 编辑 (Edit): 撤销/重做/剪切/复制/粘贴"
echo "   - 构建 (Build): 构建/重新构建/清理"
echo "   - 调试 (Debug): 下载/开始/停止/暂停"
echo "   - 工具 (Tools): 语言切换"
echo "   - 帮助 (Help)"
echo ""
echo "   工具栏: File/Edit/Build/Debug"
echo "   停靠窗口: Output/Watch/Library"
echo ""

echo "6. 使用方式..."
echo ""
echo "   用户切换语言:"
echo "   - 启动应用: ./build/NasCode"
echo "   - 菜单: 工具 → 语言 → 选择语言"
echo "   - 重启应用生效"
echo ""
echo "   开发者添加翻译:"
echo "   - 代码中使用: tr(\"Your text\")"
echo "   - 更新翻译: ./update_translations.sh"
echo "   - 编辑翻译: linguist translations/nascode_zh_CN.ts"
echo "   - 重新编译: ./build.sh"
echo ""

echo "7. 可扩展性..."
echo ""
echo "   添加新语言只需4步:"
echo "   1. 复制.ts模板文件"
echo "   2. 更新CMakeLists.txt"
echo "   3. 扩展I18nManager枚举"
echo "   4. 实现语言加载逻辑"
echo ""

echo "================================================"
echo "  ✓ i18n功能完整实现"
echo "================================================"
echo ""
echo "详细文档:"
echo "  - docs/I18N_GUIDE.md           - 完整使用指南"
echo "  - docs/I18N_IMPLEMENTATION.md  - 技术实现总结"
echo "  - translations/README.md       - 快速开始"
echo ""
