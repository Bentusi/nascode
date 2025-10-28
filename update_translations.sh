#!/bin/bash

# 更新翻译文件脚本

cd /home/wei/nascode

echo "================================================"
echo "  更新NasCode翻译文件"
echo "================================================"

# 查找所有源文件
SOURCES=$(find src -name "*.cpp" -o -name "*.h" | tr '\n' ' ')

# 更新.ts文件
echo "更新中文翻译文件..."
lupdate $SOURCES -ts translations/nascode_zh_CN.ts

echo "更新英文翻译文件..."
lupdate $SOURCES -ts translations/nascode_en_US.ts

echo ""
echo "================================================"
echo "✓ 翻译文件更新完成!"
echo "================================================"
echo ""
echo "下一步:"
echo "1. 使用Qt Linguist编辑.ts文件"
echo "2. 运行 ./build.sh 编译生成.qm文件"
echo ""
