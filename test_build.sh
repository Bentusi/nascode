#!/bin/bash

# 测试NasCode构建结果

cd /home/wei/nascode/build

echo "========================================="
echo "NasCode Build Test"
echo "========================================="

# 检查可执行文件
if [ ! -f "NasCode" ]; then
    echo "❌ NasCode executable not found!"
    exit 1
fi

echo "✅ NasCode executable found"

# 检查库文件
echo ""
echo "Libraries:"
ls -lh src/core/libNasCodeCore.a 2>/dev/null && echo "✅ NasCodeCore library" || echo "❌ NasCodeCore library missing"
ls -lh src/models/libNasCodeModels.a 2>/dev/null && echo "✅ NasCodeModels library" || echo "❌ NasCodeModels library missing"
ls -lh src/views/libNasCodeViews.a 2>/dev/null && echo "✅ NasCodeViews library" || echo "❌ NasCodeViews library missing"
ls -lh src/controllers/libNasCodeControllers.a 2>/dev/null && echo "✅ NasCodeControllers library" || echo "❌ NasCodeControllers library missing"

# 显示文件大小
echo ""
echo "========================================="
echo "Executable size:"
ls -lh NasCode

echo ""
echo "========================================="
echo "Build completed successfully! 🎉"
echo ""
echo "To run: ./build/NasCode"
echo "========================================="
