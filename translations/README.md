# NasCode 国际化 (i18n) 快速开始

## 用户指南

### 如何切换语言

1. 启动NasCode应用程序
2. 在顶部菜单栏点击 **工具 (Tools)**
3. 选择 **语言 (Language)**
4. 选择您想要的语言:
   - **English** - 英文界面
   - **Chinese** - 中文界面
5. 重启应用程序以完全生效

### 语言自动检测

首次运行时,NasCode会根据您的系统语言自动选择界面语言:
- 中文系统 → 自动使用中文界面
- 英文系统 → 自动使用英文界面
- 其他语言系统 → 使用英文界面

您的语言选择会被自动保存,下次启动时继续使用。

## 开发者指南

### 快速开始

#### 1. 在代码中添加可翻译文本

```cpp
// 使用tr()包装所有用户可见的文本
QString message = tr("Hello, World!");
setWindowTitle(tr("My Window"));
QPushButton* btn = new QPushButton(tr("Click Me"), this);
```

#### 2. 更新翻译文件

添加新文本后,运行:
```bash
./update_translations.sh
```

这会扫描所有源文件并更新翻译文件。

#### 3. 编辑翻译

##### 使用Qt Linguist (推荐)
```bash
# 安装Qt Linguist
sudo apt install qttools5-dev-tools

# 打开翻译文件
linguist translations/nascode_zh_CN.ts
```

在Qt Linguist中:
- 选择未翻译的条目
- 输入翻译文本
- 标记为"完成"
- 保存文件

##### 手动编辑.ts文件
```xml
<message>
    <source>Hello, World!</source>
    <translation>你好,世界!</translation>
</message>
```

#### 4. 编译并测试

```bash
# 编译项目(会自动生成.qm文件)
./build.sh

# 运行应用测试
./build/NasCode
```

### 支持的语言

目前支持:
- **英文** (English) - `en_US`
- **中文** (Chinese) - `zh_CN`

### 文件位置

```
nascode/
├── translations/              # 翻译源文件
│   ├── nascode_zh_CN.ts      # 中文翻译
│   └── nascode_en_US.ts      # 英文翻译
└── build/translations/        # 编译后的翻译文件
    ├── nascode_zh_CN.qm
    └── nascode_en_US.qm
```

### 常见问题

**Q: 为什么我的翻译没有生效?**

A: 请确保:
1. 已运行`./build.sh`编译生成.qm文件
2. .qm文件存在于`build/translations/`目录
3. 已重启应用程序

**Q: 如何添加新语言?**

A: 参见 [I18N_GUIDE.md](I18N_GUIDE.md) 中的详细说明。

**Q: 翻译工具在哪里下载?**

A: Qt Linguist是Qt官方工具,安装方式:
```bash
# Ubuntu/Debian
sudo apt install qttools5-dev-tools

# Fedora
sudo dnf install qt6-linguist

# 或从Qt官网下载: https://www.qt.io/
```

### 更多信息

详细文档请参阅:
- [I18N_GUIDE.md](I18N_GUIDE.md) - 完整的国际化指南
- [I18N_IMPLEMENTATION.md](I18N_IMPLEMENTATION.md) - 实现细节和技术总结
