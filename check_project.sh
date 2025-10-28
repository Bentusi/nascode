#!/bin/bash

# NasCode 项目完整性检查

echo "================================================"
echo "   NasCode 项目完整性检查"
echo "================================================"
echo ""

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

check_file() {
    if [ -f "$1" ]; then
        echo -e "${GREEN}✓${NC} $2"
        return 0
    else
        echo -e "${RED}✗${NC} $2 (缺失: $1)"
        return 1
    fi
}

check_dir() {
    if [ -d "$1" ]; then
        echo -e "${GREEN}✓${NC} $2"
        return 0
    else
        echo -e "${RED}✗${NC} $2 (缺失: $1)"
        return 1
    fi
}

cd /home/wei/nascode

# 检查文档
echo "=== 文档文件 ==="
check_file "README.md" "项目说明"
check_file "ARCHITECTURE.md" "架构文档"
check_file "COMPLETED_WORK.md" "完成工作总结"
check_file "BUILD_SUCCESS.md" "构建成功报告"
check_file "BUILD_SUMMARY.md" "构建总结"
check_file "docs/FRAMEWORK_SUMMARY.md" "框架详细总结"
check_file "docs/PLUGIN_DEVELOPMENT.md" "插件开发指南"
echo ""

# 检查构建脚本
echo "=== 构建脚本 ==="
check_file "build.sh" "构建脚本"
check_file "test_build.sh" "测试脚本"
check_file "CMakeLists.txt" "CMake主配置"
echo ""

# 检查源码
echo "=== 核心源码 ==="
check_dir "src/core" "核心层目录"
check_file "src/core/Application.h" "Application头文件"
check_file "src/core/Application.cpp" "Application实现"
check_file "src/core/EventBus.h" "EventBus头文件"
check_file "src/core/EventBus.cpp" "EventBus实现"
check_file "src/core/PluginManager.h" "PluginManager头文件"
check_file "src/core/PluginManager.cpp" "PluginManager实现"
echo ""

echo "=== Model层 ==="
check_dir "src/models" "Model层目录"
check_file "src/models/Project.h" "Project头文件"
check_file "src/models/Project.cpp" "Project实现"
check_file "src/models/POU.h" "POU头文件"
check_file "src/models/POU.cpp" "POU实现"
check_file "src/models/ProjectModel.h" "ProjectModel头文件"
check_file "src/models/ProjectModel.cpp" "ProjectModel实现"
echo ""

echo "=== View层 ==="
check_dir "src/views" "View层目录"
check_file "src/views/MainWindow.h" "MainWindow头文件"
check_file "src/views/MainWindow.cpp" "MainWindow实现"
check_file "src/views/CodeEditor.h" "CodeEditor头文件"
check_file "src/views/CodeEditor.cpp" "CodeEditor实现"
check_file "src/views/OutputPanel.h" "OutputPanel头文件"
check_file "src/views/OutputPanel.cpp" "OutputPanel实现"
echo ""

echo "=== Controller层 ==="
check_dir "src/controllers" "Controller层目录"
check_file "src/controllers/ProjectController.h" "ProjectController头文件"
check_file "src/controllers/ProjectController.cpp" "ProjectController实现"
echo ""

# 检查构建产物
echo "=== 构建产物 ==="
check_file "build/NasCode" "可执行文件"
check_file "build/src/core/libNasCodeCore.a" "核心库"
check_file "build/src/models/libNasCodeModels.a" "Model库"
check_file "build/src/views/libNasCodeViews.a" "View库"
check_file "build/src/controllers/libNasCodeControllers.a" "Controller库"
echo ""

# 检查示例
echo "=== 示例文件 ==="
check_file "examples/example_project.nsp" "示例工程"
check_file "examples/Main.st" "ST代码示例"
echo ""

# 统计信息
echo "================================================"
echo "   统计信息"
echo "================================================"
echo "源文件数量:"
echo "  - 核心层: $(find src/core -name "*.cpp" -o -name "*.h" | wc -l) 个文件"
echo "  - Model层: $(find src/models -name "*.cpp" -o -name "*.h" | wc -l) 个文件"
echo "  - View层: $(find src/views -name "*.cpp" -o -name "*.h" | wc -l) 个文件"
echo "  - Controller层: $(find src/controllers -name "*.cpp" -o -name "*.h" | wc -l) 个文件"
echo ""
echo "文档数量: $(ls -1 *.md docs/*.md 2>/dev/null | wc -l) 个"
echo ""

if [ -f "build/NasCode" ]; then
    echo "可执行文件大小: $(ls -lh build/NasCode | awk '{print $5}')"
    echo "库文件总大小: $(du -sh build/src/*/lib*.a 2>/dev/null | awk '{sum+=$1} END {print sum}')"
fi

echo ""
echo "================================================"
echo -e "${GREEN}✓ 项目完整性检查完成!${NC}"
echo "================================================"
echo ""
echo "快速命令:"
echo "  构建项目: ./build.sh"
echo "  运行程序: ./build/NasCode"
echo "  测试构建: ./test_build.sh"
echo ""
