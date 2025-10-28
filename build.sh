#!/bin/bash

# NasCode 构建脚本

set -e

echo "========================================="
echo "NasCode Build Script"
echo "========================================="

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 检查依赖
echo -e "${YELLOW}Checking dependencies...${NC}"

if ! command -v cmake &> /dev/null; then
    echo -e "${RED}CMake not found. Please install CMake 3.20 or higher.${NC}"
    exit 1
fi

if ! command -v qmake6 &> /dev/null && ! command -v qmake &> /dev/null; then
    echo -e "${RED}Qt6 not found. Please install Qt6.${NC}"
    exit 1
fi

echo -e "${GREEN}Dependencies OK${NC}"

# 创建构建目录
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Cleaning old build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 配置CMake
echo -e "${YELLOW}Configuring CMake...${NC}"
cmake -G "Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      ..

# 编译
echo -e "${YELLOW}Building...${NC}"
make -j$(nproc)

echo -e "${GREEN}=========================================${NC}"
echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "${GREEN}=========================================${NC}"
echo ""
echo "Run the application with: ./build/NasCode"
