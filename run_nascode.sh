#!/bin/bash

# NasCode 启动脚本
# 用于测试新建项目功能

echo "==================================="
echo "  NasCode - IEC-61131 ST IDE"
echo "==================================="
echo ""
echo "启动应用..."
echo ""
echo "测试步骤:"
echo "1. 点击菜单 File -> New Project 或按 Ctrl+N"
echo "2. 输入项目名称(如: TestProject)"
echo "3. 选择保存位置"
echo "4. 确认项目创建成功"
echo ""
echo "预期行为:"
echo "✓ 弹出输入框要求输入项目名称"
echo "✓ 弹出文件保存对话框"
echo "✓ 项目文件(.nascode)创建"
echo "✓ 状态栏显示: Project created: 项目名"
echo "✓ 项目树显示项目结构"
echo ""
echo "==================================="
echo ""

# 启动应用
cd /home/wei/nascode
./build/NasCode
