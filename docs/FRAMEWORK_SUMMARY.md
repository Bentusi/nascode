# NasCode 核心框架架构总结

## 📋 技术选型

### ✅ 采用的成熟框架

| 框架/库 | 用途 | 优势 |
|---------|------|------|
| **Qt6 Model/View** | MVC架构基础 | 成熟稳定、信号槽机制、跨平台 |
| **Qt Plugin System** | 插件管理 | 动态加载、元对象系统、接口定义 |
| **spdlog** | 日志系统 | 高性能、多sink、格式化 |
| **Qt6::SerialPort** | 串口通讯 | 跨平台、异步IO |
| **Qt6::Network** | 网络通讯 | TCP/UDP、HTTP支持 |
| **QSettings** | 配置管理 | 跨平台、INI/Registry |

### 🔄 推荐集成的框架

| 框架/库 | 用途 | 集成优先级 |
|---------|------|-----------|
| **QScintilla** | 代码编辑器 | ⭐⭐⭐⭐⭐ 高 |
| **ANTLR4** | 编译器前端 | ⭐⭐⭐⭐⭐ 高 |
| **nlohmann/json** | JSON解析 | ⭐⭐⭐⭐ 中高 |
| **Google Test** | 单元测试 | ⭐⭐⭐ 中 |
| **libmodbus** | Modbus协议 | ⭐⭐⭐ 中 (插件) |
| **open62541** | OPC UA | ⭐⭐ 低 (插件) |

## 🏗️ MVC架构设计

```
┌─────────────────────────────────────────────────────────────┐
│                        Application                          │
│                      (应用程序核心)                           │
│  - 单例模式                                                  │
│  - 生命周期管理                                              │
│  - 全局服务提供                                              │
└────────────┬────────────────────────────┬────────────────────┘
             │                            │
    ┌────────▼────────┐          ┌───────▼──────────┐
    │   EventBus      │          │  PluginManager   │
    │   (事件总线)     │          │  (插件管理器)      │
    │  - 发布/订阅     │          │  - 动态加载       │
    │  - 解耦通信     │          │  - 生命周期管理    │
    └─────────────────┘          └──────────────────┘

┌──────────────────────────────────────────────────────────────┐
│                          MVC 层次                             │
├─────────────────┬──────────────────┬─────────────────────────┤
│     Model       │    Controller    │         View            │
│   (数据模型)     │   (业务控制)      │      (用户界面)          │
├─────────────────┼──────────────────┼─────────────────────────┤
│ ProjectModel    │ ProjectController│ MainWindow              │
│  - QAbstractItem│  - 协调M-V       │  - 主窗口               │
│    Model        │  - 用户交互      │  - 菜单/工具栏          │
│  - 树形结构     │  - 业务流程      │  - 停靠窗口             │
│                 │                  │                         │
│ Project         │ BuildController  │ CodeEditor              │
│  - 工程数据     │  - 编译控制      │  - 语法高亮             │
│  - 序列化       │  - 错误处理      │  - 行号显示             │
│                 │                  │  - 代码折叠             │
│ POU             │ DebugController  │                         │
│  - Program      │  - 调试控制      │ OutputPanel             │
│  - Function     │  - 断点管理      │  - 构建输出             │
│  - FunctionBlock│  - 变量监视      │  - 调试输出             │
│                 │                  │                         │
│ Variable        │ CommController   │ ProjectTreeView         │
│  - 变量定义     │  - 设备通讯      │  - 工程树               │
│  - 类型系统     │  - 下载/上传     │  - 右键菜单             │
└─────────────────┴──────────────────┴─────────────────────────┘

        ▲                   ▲                    ▲
        │                   │                    │
        │ 使用              │ 调用               │ 显示
        │                   │                    │
┌───────┴───────────────────┴────────────────────┴──────────┐
│                      底层服务                              │
├────────────────┬──────────────────┬────────────────────────┤
│  Compiler      │  Communication   │      Plugins           │
│  (编译器)       │   (通讯模块)      │     (插件)              │
├────────────────┼──────────────────┼────────────────────────┤
│ - Lexer        │ - SerialPort     │ - LD Editor            │
│ - Parser       │ - TCP/UDP        │ - FBD Editor           │
│ - CodeGen      │ - STVM Protocol  │ - SFC Editor           │
│ - Optimizer    │ - Modbus (插件)  │ - HMI Designer         │
│                │ - OPC UA (插件)  │ - Trend Viewer         │
└────────────────┴──────────────────┴────────────────────────┘
```

## 📂 目录结构与职责

```
src/
├── core/                          # 核心层
│   ├── Application.h/cpp         # 应用程序单例
│   ├── EventBus.h/cpp            # 事件总线
│   ├── PluginManager.h/cpp       # 插件管理
│   └── IPlugin.h                 # 插件接口
│
├── models/                        # Model层(数据)
│   ├── Project.h/cpp             # 工程数据
│   ├── POU.h/cpp                 # POU数据
│   ├── ProjectModel.h/cpp        # Qt Model实现
│   └── ...                       # 其他数据模型
│
├── views/                         # View层(界面)
│   ├── MainWindow.h/cpp          # 主窗口
│   ├── CodeEditor.h/cpp          # 代码编辑器
│   ├── OutputPanel.h/cpp         # 输出面板
│   └── ...                       # 其他视图组件
│
├── controllers/                   # Controller层(逻辑)
│   ├── ProjectController.h/cpp   # 工程控制
│   ├── BuildController.h/cpp     # 编译控制(待实现)
│   ├── DebugController.h/cpp     # 调试控制(待实现)
│   └── ...                       # 其他控制器
│
├── compiler/                      # 编译器
│   ├── lexer/                    # 词法分析
│   ├── parser/                   # 语法分析
│   └── codegen/                  # 代码生成
│
├── communication/                 # 通讯模块
│   ├── serial/                   # 串口
│   └── tcp/                      # TCP/UDP
│
└── plugins/                       # 插件
    ├── example/                  # 示例插件
    └── ...                       # 其他插件
```

## 🔄 数据流与控制流

### 用户操作流程(以"打开工程"为例)

```
User Action                 View                Controller              Model
    │                        │                      │                    │
    │  点击"Open Project"    │                      │                    │
    ├───────────────────────>│                      │                    │
    │                        │  onOpenProject()     │                    │
    │                        ├─────────────────────>│                    │
    │                        │                      │ openProject()      │
    │                        │                      ├───────────────────>│
    │                        │                      │                    │
    │                        │                      │   loadFromFile()   │
    │                        │                      │<───────────────────┤
    │                        │                      │                    │
    │                        │   projectOpened()    │                    │
    │                        │<─────────────────────┤                    │
    │                        │                      │                    │
    │    updateUI()          │                      │                    │
    │<───────────────────────┤                      │                    │
    │                        │                      │                    │
```

### 事件总线通讯

```
Component A                 EventBus              Component B
    │                          │                       │
    │  publish("build.start")  │                       │
    ├─────────────────────────>│                       │
    │                          │   notify              │
    │                          ├──────────────────────>│
    │                          │                       │
    │                          │   callback()          │
    │                          │<──────────────────────┤
    │                          │                       │
```

## 🔌 插件系统设计

### 插件生命周期

```
┌──────────────┐
│ Plugin.so    │
└──────┬───────┘
       │
       │ PluginManager::loadPlugin()
       ▼
┌──────────────┐
│ QPluginLoader│
│   .instance()│
└──────┬───────┘
       │
       │ qobject_cast<IPlugin*>
       ▼
┌──────────────┐
│   IPlugin    │
│ .initialize()│────────> 订阅事件、注册服务
└──────┬───────┘
       │
       │ 运行中
       ▼
┌──────────────┐
│   Active     │────────> 处理事件、提供功能
└──────┬───────┘
       │
       │ Application关闭
       ▼
┌──────────────┐
│  .shutdown() │────────> 清理资源
└──────┬───────┘
       │
       │ PluginManager::unloadPlugin()
       ▼
┌──────────────┐
│   Unloaded   │
└──────────────┘
```

## 🎯 关键设计模式

| 模式 | 应用位置 | 目的 |
|------|----------|------|
| **Singleton** | Application | 全局唯一实例 |
| **MVC** | 整体架构 | 分离关注点 |
| **Observer** | EventBus | 发布订阅 |
| **Plugin** | PluginManager | 动态扩展 |
| **Factory** | POU创建 | 对象创建 |
| **Strategy** | 编译器 | 算法替换 |

## 📊 类关系图(核心部分)

```
                    ┌─────────────┐
                    │ Application │
                    │  (单例)      │
                    └──────┬──────┘
                           │ owns
              ┌────────────┼────────────┐
              │                         │
      ┌───────▼────────┐       ┌───────▼──────┐
      │   EventBus     │       │ PluginManager│
      └────────────────┘       └──────┬───────┘
                                      │ manages
                                      │
                               ┌──────▼──────┐
                               │   IPlugin   │ <<interface>>
                               └──────▲──────┘
                                      │ implements
                         ┌────────────┼────────────┐
                         │            │            │
                  ┌──────┴──────┐ ┌──┴──┐   ┌────┴────┐
                  │  LDPlugin   │ │FBD  │   │SFC      │
                  └─────────────┘ └─────┘   └─────────┘


      ┌──────────────┐         ┌──────────────┐
      │ProjectModel  │         │  Project     │
      │<<QAbstractI..│◆────────│              │
      └──────────────┘ owns    └──────┬───────┘
                                      │ contains
                                      │
                               ┌──────▼──────┐
                               │    POU      │ <<abstract>>
                               └──────▲──────┘
                                      │ inherits
                         ┌────────────┼────────────┐
                         │            │            │
                  ┌──────┴──────┐ ┌──┴───────┐ ┌─┴────────┐
                  │  Program    │ │ Function │ │Function  │
                  │             │ │          │ │Block     │
                  └─────────────┘ └──────────┘ └──────────┘
```

## ✨ MVC架构的优势体现

### 1. **可测试性**
```cpp
// Model 可以独立测试,无需UI
void testProjectModel() {
    ProjectModel model;
    model.createProject("Test", "/tmp");
    ASSERT_TRUE(model.currentProject() != nullptr);
}
```

### 2. **可替换性**
```cpp
// 可以轻松切换View实现
MainWindow* view1 = new MainWindow();  // Qt Widgets
MainWindow* view2 = new QMLMainWindow(); // Qt QML
// Controller不需要修改
```

### 3. **可复用性**
```cpp
// 同一个Model可以被多个View使用
ProjectModel* model = new ProjectModel();
TreeView* treeView = new TreeView(model);
ListView* listView = new ListView(model);
```

### 4. **易维护性**
- Model修改不影响View
- View修改不影响Model
- Controller协调逻辑清晰

## 🚀 下一步集成建议

### 优先级1: QScintilla集成
```cpp
// 替换 CodeEditor 为 QsciScintilla
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h> // 或自定义ST lexer

class CodeEditor : public QsciScintilla {
    // ... 更强大的编辑功能
};
```

### 优先级2: ANTLR4语法定义
```antlr
grammar ST;

program
    : PROGRAM ID declarations body END_PROGRAM
    ;

declarations
    : VAR variable_list END_VAR
    ;

// ... ST语言语法规则
```

### 优先级3: 完善Controller
```cpp
class BuildController : public QObject {
    // 编译流程控制
    bool compile();
    void handleErrors();
};

class DebugController : public QObject {
    // 调试流程控制
    void startDebugging();
    void setBreakpoint();
    void watchVariable();
};
```

## 📝 总结

该框架采用了**成熟的Qt MVC架构**,结合**插件系统**实现高度的:
- ✅ **模块化** - 清晰的层次划分
- ✅ **可扩展** - 插件机制
- ✅ **可维护** - MVC分离关注点
- ✅ **可测试** - Model独立于UI
- ✅ **跨平台** - Qt6支持

核心代码已经搭建完成,可以在此基础上:
1. 集成更强大的第三方组件(QScintilla, ANTLR4)
2. 实现编译器和通讯模块
3. 开发插件生态
