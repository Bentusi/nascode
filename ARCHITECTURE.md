# NasCode - IEC-61131 ST Programming Environment

基于Qt6和MVC架构的工业控制编程上位机

## 架构设计

### 核心框架
- **Qt6 Model/View** - 利用Qt成熟的MVC框架
- **插件系统** - 基于Qt插件机制的可扩展架构
- **事件总线** - 组件间解耦通信

### 目录结构

```
nascode/
├── src/
│   ├── main.cpp                 # 程序入口
│   ├── core/                    # 核心层
│   │   ├── Application.h/cpp   # 应用程序单例
│   │   ├── EventBus.h/cpp      # 事件总线
│   │   ├── PluginManager.h/cpp # 插件管理器
│   │   └── IPlugin.h           # 插件接口
│   ├── models/                  # 模型层(MVC-Model)
│   │   ├── Project.h/cpp       # 工程数据模型
│   │   ├── POU.h/cpp           # POU数据模型
│   │   └── ProjectModel.h/cpp  # Qt Model实现
│   ├── views/                   # 视图层(MVC-View)
│   │   ├── MainWindow.h/cpp    # 主窗口
│   │   ├── CodeEditor.h/cpp    # 代码编辑器
│   │   └── OutputPanel.h/cpp   # 输出面板
│   ├── controllers/             # 控制器层(MVC-Controller)
│   │   └── ProjectController.h/cpp
│   ├── compiler/                # 编译器
│   │   ├── lexer/              # 词法分析
│   │   ├── parser/             # 语法分析
│   │   └── codegen/            # 代码生成
│   ├── communication/           # 通讯模块
│   │   ├── serial/             # 串口通讯
│   │   └── tcp/                # TCP/UDP通讯
│   └── plugins/                 # 插件
├── CMakeLists.txt
└── README.md
```

## 可复用的成熟框架

### 1. Qt Model/View (内置)
- **QAbstractItemModel** - 树形/列表数据模型基类
- **QTreeView/QTableView** - 内置视图组件
- **优势**: 高度优化、信号槽机制、数据视图分离

### 2. 编辑器组件
- **QScintilla** (推荐) - 功能强大的代码编辑器
  - 语法高亮、代码折叠
  - 自动补全、代码提示
  - 行号、断点标记
  - 多语言支持
- 或使用 **KTextEditor** (KDE框架)

### 3. 编译器前端
- **ANTLR4** - 强大的词法/语法分析器生成器
  - 支持LL(*)算法
  - 可视化语法树
  - 多语言目标
- **Flex/Bison** - 经典工具
- **手写递归下降解析器** - 更灵活

### 4. 通讯库
- **Qt6::SerialPort** - 串口通讯
- **Qt6::Network** - TCP/UDP/HTTP
- **Modbus库**: libmodbus
- **OPC UA**: open62541

### 5. 日志系统
- **spdlog** - 高性能C++日志库
- **Qt日志系统** - QDebug/QLoggingCategory

### 6. 配置管理
- **QSettings** - Qt内置配置
- **nlohmann/json** - JSON解析
- **YAML-cpp** - YAML支持

### 7. 测试框架
- **Google Test** - 单元测试
- **Catch2** - 现代C++测试
- **Qt Test** - Qt集成测试

## MVC架构优势

### Model(模型层)
- 纯数据和业务逻辑
- 独立于界面
- 可复用、可测试
- **示例**: `ProjectModel`, `Project`, `POU`

### View(视图层)
- 纯UI展示
- 不包含业务逻辑
- Qt Designer可视化设计
- **示例**: `MainWindow`, `CodeEditor`, `OutputPanel`

### Controller(控制器层)
- 协调Model和View
- 处理用户交互
- 业务流程控制
- **示例**: `ProjectController`

## 构建说明

### 依赖安装

```bash
# Ubuntu/Debian
sudo apt install qt6-base-dev libspdlog-dev cmake ninja-build

# Fedora
sudo dnf install qt6-qtbase-devel spdlog-devel cmake ninja-build

# macOS
brew install qt@6 spdlog cmake ninja

# Windows
# 下载Qt6安装包，使用vcpkg安装spdlog
```

### 编译

```bash
mkdir build && cd build
cmake -G Ninja ..
ninja
```

### 运行

```bash
./NasCode
```

## 下一步开发计划

### Phase 1: 核心框架(当前)
- [x] MVC架构搭建
- [x] 插件系统基础
- [x] 事件总线
- [ ] 基础代码编辑器
- [ ] 工程文件管理

### Phase 2: 编译器
- [ ] ST语言词法分析器(ANTLR4)
- [ ] 语法分析器
- [ ] 语义分析
- [ ] IR生成
- [ ] STVM字节码生成

### Phase 3: 通讯与调试
- [ ] 串口/TCP通讯协议
- [ ] 程序下载
- [ ] 在线调试
- [ ] 变量监视/强制

### Phase 4: 插件生态
- [ ] 图形化编程插件(LD/FBD/SFC)
- [ ] HMI设计器
- [ ] 数据趋势图
- [ ] 第三方协议支持

## 贡献指南

欢迎贡献代码、报告Bug、提出建议!

## 许可证

待定

## 联系方式

待定
