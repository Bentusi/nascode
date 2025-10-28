# NasCode 代码编辑器核心功能文档

## 功能概述

NasCode的代码编辑器提供了专业的ST语言编程体验,包含智能提示、自动补全、代码片段、语法高亮等现代IDE必备功能。

## 核心特性

### 1. 语法高亮 ✅

**STSyntaxHighlighter** - 完整的ST语言语法高亮

- **关键字高亮** (蓝色加粗):
  - 程序结构: `PROGRAM`, `FUNCTION`, `FUNCTION_BLOCK`, `END_*`
  - 变量声明: `VAR`, `VAR_INPUT`, `VAR_OUTPUT`, `VAR_IN_OUT`, `VAR_TEMP`
  - 控制流: `IF`, `THEN`, `ELSE`, `ELSIF`, `CASE`, `FOR`, `WHILE`, `REPEAT`
  - 逻辑运算: `AND`, `OR`, `XOR`, `NOT`, `MOD`

- **数据类型高亮** (深青色加粗):
  - 布尔和位: `BOOL`, `BYTE`, `WORD`, `DWORD`, `LWORD`
  - 整数: `SINT`, `INT`, `DINT`, `LINT`, `USINT`, `UINT`, `UDINT`, `ULINT`
  - 浮点: `REAL`, `LREAL`
  - 时间: `TIME`, `DATE`, `TOD`, `DT`
  - 字符串: `STRING`, `WSTRING`

- **注释高亮** (灰色斜体):
  - 单行注释: `// 注释内容`
  - 多行注释: `(* 注释内容 *)`

- **数字高亮** (深绿色):  
  整数和浮点数自动识别

- **字符串高亮** (深红色):  
  单引号字符串 `'Hello'`

### 2. 智能代码补全 ✅

**CodeCompleter** - 上下文感知的代码补全系统

#### 自动补全
- **触发方式**:
  - 自动触发: 输入2个字符后300ms自动显示
  - 手动触发: `Ctrl+Space`

- **补全内容**:
  - 所有ST语言关键字 (40+)
  - 数据类型关键字 (20+)
  - 标准函数库 (30+)
    - 数学: `ABS`, `SQRT`, `SIN`, `COS`, `TAN`
    - 类型转换: `INT_TO_REAL`, `REAL_TO_INT`
    - 字符串: `LEN`, `LEFT`, `RIGHT`, `CONCAT`
    - 比较: `MAX`, `MIN`, `LIMIT`

- **智能特性**:
  - 大小写不敏感
  - 前缀匹配
  - 弹出式列表
  - 方向键选择
  - Enter/Tab确认

#### 使用示例
```st
// 输入 "pro" 自动提示:
PROGRAM
PROGRAM_CYCLE

// 输入 "int" 自动提示:
INT
INT_TO_REAL
INT_TO_STRING
```

### 3. 代码片段 (Snippets) ✅

**SnippetManager** - 快速插入代码模板

#### 内置片段

| 触发词 | 描述 | 展开内容 |
|--------|------|----------|
| `prog` | 程序模板 | `PROGRAM ... END_PROGRAM` |
| `func` | 函数模板 | `FUNCTION ... END_FUNCTION` |
| `fb` | 功能块模板 | `FUNCTION_BLOCK ... END_FUNCTION_BLOCK` |
| `if` | IF语句 | `IF ... THEN ... END_IF` |
| `ife` | IF-ELSE语句 | `IF ... THEN ... ELSE ... END_IF` |
| `for` | FOR循环 | `FOR i := 0 TO 10 BY 1 DO ... END_FOR` |
| `while` | WHILE循环 | `WHILE ... DO ... END_WHILE` |
| `case` | CASE语句 | `CASE ... OF ... END_CASE` |
| `var` | 变量块 | `VAR ... END_VAR` |
| `struct` | 结构体 | `TYPE ... STRUCT ... END_STRUCT` |

#### 使用方式
1. 输入触发词 (如 `for`)
2. 按 `Tab` 键
3. 代码片段自动展开,光标定位到编辑位置

#### 示例
```st
// 输入: for <Tab>
// 展开为:
FOR i := 0 TO 10 BY 1 DO
    |← 光标在这里
END_FOR
```

### 4. 行号显示 ✅

**LineNumberArea** - 左侧行号区域

- 自动计算宽度适应行数
- 当前行高亮
- 浅灰色背景
- 右对齐显示

### 5. 自动缩进 ✅

**Auto-Indent** - 智能缩进管理

#### 特性
- **Enter键自动缩进**: 保持上一行缩进级别
- **智能增加缩进**: 检测 `THEN`, `DO`, `STRUCT`, `VAR` 等关键字
- **Tab键**: 插入4个空格(可配置)
- **手动缩进控制**:
  - `Ctrl+]`: 增加缩进
  - `Ctrl+[`: 减少缩进

#### 示例
```st
IF condition THEN  ← Enter后
    |← 自动缩进到这里
```

### 6. 括号匹配 ✅

**Bracket Matching** - 括号高亮显示

#### 支持的括号
- 圆括号: `()`
- 方括号: `[]`
- 花括号: `{}`

#### 特性
- 实时高亮匹配的括号对
- 黄色背景标记
- 光标前后字符都检测
- 嵌套括号正确匹配

#### 自动配对
输入左括号时自动插入右括号:
```st
输入: (
结果: (|)  ← 光标在中间
```

### 7. 注释切换 ✅

**Comment Toggle** - 快速添加/删除注释

#### 快捷键
`Ctrl+/` - 切换当前行或选中行的注释

#### 行为
- **未注释** → 添加 `//`
- **已注释** → 移除 `//`
- **多行选择** → 批量处理

#### 示例
```st
// 选中多行后按 Ctrl+/
x := 1;        →  //x := 1;
y := 2;        →  //y := 2;

// 再按一次恢复
//x := 1;      →  x := 1;
//y := 2;      →  y := 2;
```

### 8. 当前行高亮 ✅

- 浅黄色背景标记当前编辑行
- 自动跟随光标位置
- 不影响文本选择

### 9. 字体配置 ✅

- **默认字体**: Consolas 10pt
- **等宽字体**: 代码对齐美观
- **Tab宽度**: 4个空格

## 快捷键总览

| 快捷键 | 功能 |
|--------|------|
| `Tab` | 展开代码片段 / 插入缩进 |
| `Ctrl+Space` | 手动触发代码补全 |
| `Ctrl+/` | 切换注释 |
| `Ctrl+]` | 增加缩进 |
| `Ctrl+[` | 减少缩进 |
| `Enter` | 智能自动缩进 |

## 配置选项

编辑器支持的配置(在代码中可修改):

```cpp
bool m_autoIndentEnabled = true;         // 自动缩进
bool m_bracketMatchingEnabled = true;    // 括号匹配
bool m_autoCompletionEnabled = true;     // 自动补全
int m_tabSize = 4;                       // Tab大小
```

## 代码示例

### 完整的ST程序示例
```st
PROGRAM ExampleProgram
VAR
    counter : INT := 0;
    result : REAL;
    flag : BOOL;
END_VAR

// 主循环
FOR counter := 0 TO 10 BY 1 DO
    IF counter MOD 2 = 0 THEN
        result := INT_TO_REAL(counter) * 1.5;
    ELSE
        result := 0.0;
    END_IF;
END_FOR;

END_PROGRAM
```

在编辑器中,这段代码会显示:
- **PROGRAM**, **VAR**, **FOR**, **IF** 等关键字高亮(蓝色)
- **INT**, **REAL**, **BOOL** 类型高亮(青色)
- 数字 **0**, **10**, **1**, **2**, **1.5**, **0.0** 高亮(绿色)
- 注释 **// 主循环** 高亮(灰色)
- 当前编辑行黄色背景
- 行号在左侧显示
- 括号自动配对和高亮

## 性能

- **语法高亮**: 实时,逐行处理,无延迟
- **代码补全**: 300ms防抖,减少不必要的弹窗
- **括号匹配**: 仅在光标移动时计算,O(n)复杂度
- **内存占用**: 轻量级,补全列表约100个词条

## 未来扩展

### 计划中的功能
- [ ] **代码折叠**: 折叠/展开代码块
- [ ] **多光标编辑**: 同时编辑多处
- [ ] **查找/替换**: 高级搜索功能
- [ ] **代码格式化**: 自动格式化代码
- [ ] **错误标记**: 实时语法检查
- [ ] **智能重构**: 重命名变量/函数
- [ ] **代码导航**: 跳转到定义
- [ ] **参数提示**: 函数调用提示
- [ ] **自定义片段**: 用户定义代码模板
- [ ] **代码模板**: 工程模板库

## 架构设计

### 类层次结构

```
CodeEditor (QPlainTextEdit)
    ├── LineNumberArea (QWidget)
    ├── STSyntaxHighlighter (QSyntaxHighlighter)
    ├── CodeCompleter
    │   ├── QCompleter
    │   └── QStringListModel
    └── SnippetManager
        └── QMap<QString, CodeSnippet>
```

### 文件组织

```
src/views/
├── CodeEditor.h             # 编辑器核心类
├── CodeEditor.cpp           # 编辑器实现(500行)
├── CodeCompleter.h          # 补全系统接口
└── CodeCompleter.cpp        # 补全系统实现(250行)
```

### 信号与槽

```cpp
// 文本变化 → 触发补全
connect(textChanged, onTextChanged)
    → completionTimer->start()
    → checkAutoCompletion()
    → showCompletion()

// 光标移动 → 高亮当前行 + 括号匹配
connect(cursorPositionChanged, highlightCurrentLine)
connect(cursorPositionChanged, highlightMatchingBrackets)

// 补全选择 → 插入文本
connect(completer->activated, insertCompletion)
```

## 使用指南

### 开发者集成

```cpp
// 创建编辑器
auto* editor = new CodeEditor(this);

// 设置文件
editor->setFilePath("program.st");

// 加载内容
QFile file("program.st");
if (file.open(QIODevice::ReadOnly)) {
    editor->setPlainText(file.readAll());
}

// 获取内容
QString code = editor->toPlainText();

// 跳转到行
editor->gotoLine(42);

// 切换注释
editor->toggleComment();
```

### 自定义补全

```cpp
// 添加自定义补全项(如变量名)
QStringList customItems = {"myVariable", "myFunction"};
editor->completer()->updateCompletions(customItems);
```

### 自定义代码片段

```cpp
// 添加自定义片段
CodeSnippet snippet;
snippet.trigger = "mysnippet";
snippet.description = "My custom snippet";
snippet.code = "// Custom code\n\t";
snippet.cursorOffset = 15;
editor->snippetManager()->addSnippet(snippet);
```

## 测试

### 手动测试清单

- [x] 语法高亮正确显示所有类型
- [x] 代码补全在输入时弹出
- [x] Tab键展开代码片段
- [x] Ctrl+Space手动触发补全
- [x] Ctrl+/切换注释
- [x] Enter键自动缩进
- [x] 括号自动配对
- [x] 括号匹配高亮
- [x] 行号正确显示
- [x] 当前行高亮

### 性能测试

- 测试大文件(1000+行)语法高亮性能
- 测试快速输入时补全响应
- 测试大量嵌套括号匹配性能

## 总结

NasCode的代码编辑器提供了专业的ST语言编程体验,包含:

- ✅ **完整语法高亮** - 关键字、类型、注释、字符串、数字
- ✅ **智能代码补全** - 40+关键字, 30+标准函数, 自动/手动触发
- ✅ **代码片段系统** - 10+内置模板, 快速展开
- ✅ **行号显示** - 自适应宽度
- ✅ **自动缩进** - 智能识别结构
- ✅ **括号匹配** - 实时高亮, 自动配对
- ✅ **注释切换** - 单行/多行支持
- ✅ **现代编辑体验** - 快捷键支持, 配置灵活

总计约**750行**核心代码,提供现代IDE级别的编辑体验!

---

**版本**: v1.0  
**状态**: ✅ 完成并测试  
**日期**: 2025年10月28日
