# NasCode 国际化(i18n)支持文档

## 概述

NasCode现已支持多语言界面,默认支持中文和英文,并可轻松扩展到其他语言。

## 支持的语言

- **English** (英文) - en_US
- **Chinese** (中文) - zh_CN

## 使用方式

### 用户切换语言

1. 启动NasCode
2. 点击菜单 **工具 (Tools)** → **语言 (Language)**
3. 选择您想要的语言
4. 重启应用程序以完全生效

### 自动语言检测

首次运行时,NasCode会自动检测系统语言:
- 中文系统 → 自动使用中文界面
- 其他系统 → 自动使用英文界面

语言设置会保存,下次启动时使用上次的选择。

## 开发者指南

### 添加新的可翻译文本

在代码中使用`tr()`函数标记需要翻译的文本:

```cpp
// 示例
QString message = tr("Hello, World!");
QAction* action = new QAction(tr("&Open File"), this);
setWindowTitle(tr("NasCode - Programming Environment"));
```

### 更新翻译文件

当添加了新的可翻译文本后:

```bash
# 运行翻译更新脚本
./update_translations.sh
```

这会更新所有.ts翻译文件。

### 编辑翻译

使用Qt Linguist编辑翻译:

```bash
# 安装Qt Linguist
sudo apt install qttools5-dev-tools  # Ubuntu/Debian
sudo dnf install qt6-linguist         # Fedora

# 打开翻译文件
linguist translations/nascode_zh_CN.ts
```

在Qt Linguist中:
1. 选择未翻译的条目
2. 在"翻译"栏输入翻译文本
3. 标记为"完成"
4. 保存文件

### 编译翻译文件

```bash
# 编译生成.qm文件
./build.sh
```

编译后的.qm文件会自动复制到`build/translations/`目录。

### 添加新语言

1. **创建新的.ts文件**
```bash
cd translations
cp nascode_en_US.ts nascode_ja_JP.ts  # 日语示例
```

2. **编辑CMakeLists.txt**
```cmake
set(TS_FILES
    ${CMAKE_SOURCE_DIR}/translations/nascode_zh_CN.ts
    ${CMAKE_SOURCE_DIR}/translations/nascode_en_US.ts
    ${CMAKE_SOURCE_DIR}/translations/nascode_ja_JP.ts  # 新增
)
```

3. **更新I18nManager**

在`src/core/I18nManager.h`中添加新语言:
```cpp
enum Language {
    English,
    Chinese,
    Japanese,  // 新增
    SystemDefault
};
```

在`src/core/I18nManager.cpp`中实现:
```cpp
case Japanese:
    translationFile = "nascode_ja_JP";
    qtTranslationFile = "qt_ja";
    break;
```

4. **更新翻译并编译**
```bash
./update_translations.sh
linguist translations/nascode_ja_JP.ts
./build.sh
```

## 翻译文件结构

### .ts 文件 (源翻译文件)
- XML格式
- 可用Qt Linguist编辑
- 包含源文本和翻译文本
- 存储在`translations/`目录

### .qm 文件 (编译后的翻译文件)
- 二进制格式
- 运行时加载
- 由.ts文件编译生成
- 存储在`build/translations/`目录

## 架构设计

### I18nManager类

单例模式的国际化管理器:

```cpp
// 获取实例
auto& i18n = I18nManager::getInstance();

// 初始化
i18n.initialize();

// 切换语言
i18n.switchLanguage(I18nManager::Chinese);

// 获取当前语言
auto lang = i18n.currentLanguage();

// 监听语言变化
connect(&i18n, &I18nManager::languageChanged, 
        this, &MyWidget::onLanguageChanged);
```

### 动态语言切换

实现`changeEvent()`以支持实时语言切换:

```cpp
void MyWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();  // 重新翻译界面
    }
    QWidget::changeEvent(event);
}

void MyWidget::retranslateUi()
{
    setWindowTitle(tr("My Widget"));
    m_button->setText(tr("Click Me"));
    // 更新所有界面文本
}
```

## 最佳实践

### 1. 使用tr()包装所有用户可见文本

✅ 好的做法:
```cpp
QString msg = tr("File saved successfully");
```

❌ 不好的做法:
```cpp
QString msg = "File saved successfully";  // 无法翻译
```

### 2. 为复数形式提供支持

```cpp
QString msg = tr("%n file(s) found", "", fileCount);
```

### 3. 提供上下文

```cpp
// 在.cpp文件中
QString text = tr("Open");  // 按钮文本

// Qt会自动使用类名作为上下文
// 在.ts文件中显示为: nascode::views::MainWindow
```

### 4. 使用注释为翻译者提供上下文

```cpp
//: 这是工具栏的打开按钮
QString text = tr("Open");
```

### 5. 避免字符串拼接

❌ 不好:
```cpp
QString msg = tr("Hello") + ", " + userName;  // 无法正确翻译
```

✅ 好的:
```cpp
QString msg = tr("Hello, %1").arg(userName);
```

## 测试翻译

### 测试中文界面
```bash
./build/NasCode
# 工具 → 语言 → Chinese
```

### 测试英文界面
```bash
./build/NasCode
# Tools → Language → English
```

### 使用特定语言启动
```bash
# 设置环境变量
LANG=zh_CN.UTF-8 ./build/NasCode
LANG=en_US.UTF-8 ./build/NasCode
```

## 翻译进度追踪

使用lrelease查看翻译完成度:

```bash
lrelease -verbose translations/nascode_zh_CN.ts
```

输出示例:
```
Updating 'nascode_zh_CN.qm'...
    Generated 45 translation(s) (45 finished and 0 unfinished)
    Ignored 0 untranslated source text(s)
```

## 常见问题

### Q: 翻译不生效怎么办?

A: 检查以下几点:
1. 确保运行了`./build.sh`编译.qm文件
2. 确保.qm文件在正确的位置(`build/translations/`)
3. 重启应用程序

### Q: 如何查看未翻译的文本?

A: 使用Qt Linguist:
1. 打开.ts文件
2. 选择"视图" → "未完成的翻译"
3. 逐个翻译并标记为完成

### Q: 为什么有些Qt标准对话框没有翻译?

A: 需要确保Qt的翻译文件也被加载:
```cpp
// I18nManager会自动加载Qt翻译
m_qtTranslator->load(qtTranslationFile, qtTransPath);
```

## 文件清单

```
nascode/
├── translations/              # 翻译文件目录
│   ├── nascode_zh_CN.ts      # 中文翻译源文件
│   ├── nascode_en_US.ts      # 英文翻译源文件
│   └── README.md             # 翻译说明
├── src/core/
│   ├── I18nManager.h         # 国际化管理器头文件
│   └── I18nManager.cpp       # 国际化管理器实现
├── update_translations.sh     # 更新翻译脚本
└── build/translations/        # 编译后的.qm文件
    ├── nascode_zh_CN.qm
    └── nascode_en_US.qm
```

## 贡献翻译

欢迎贡献新的语言翻译!

1. Fork项目
2. 添加新语言的.ts文件
3. 完成翻译
4. 提交Pull Request

翻译质量要求:
- 准确性: 忠实于原文意思
- 专业性: 使用正确的专业术语
- 一致性: 保持术语翻译一致
- 简洁性: 界面文本要简洁明了

## 相关资源

- [Qt Linguist手册](https://doc.qt.io/qt-6/qtlinguist-index.html)
- [Qt国际化指南](https://doc.qt.io/qt-6/internationalization.html)
- [Qt翻译格式](https://doc.qt.io/qt-6/linguist-ts-file-format.html)

---

**提示**: 良好的国际化支持能让您的应用服务更广泛的用户群体!
