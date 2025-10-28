# 项目管理功能 - 快速上手

## 功能清单 ✅

### 已实现功能

#### 1. 项目配置管理
- [x] 目标设备配置
- [x] 编译器设置
- [x] 优化选项 (级别0-3)
- [x] 调试信息开关
- [x] 串口通信配置
- [x] 网络通信配置
- [x] JSON序列化

#### 2. 库管理系统
- [x] 库路径管理
- [x] 4个标准库内置 (Standard, String, Math, Communication)
- [x] 库扫描和发现
- [x] 库引用添加/删除
- [x] 库元数据存储
- [x] 库启用/禁用控制

#### 3. 项目增强
- [x] 作者和描述字段
- [x] 创建/修改时间追踪
- [x] 自动modified标志
- [x] 按类型查询POU
- [x] 项目统计信息
- [x] 完整JSON序列化

#### 4. 控制器扩展
- [x] 添加库引用
- [x] 移除库引用
- [x] 获取可用库列表
- [x] 获取项目统计

### 待实现功能

#### 1. POU序列化
- [ ] POU对象转JSON
- [ ] JSON转POU对象

#### 2. 导入导出
- [ ] ZIP打包导出
- [ ] ZIP解包导入
- [ ] 项目模板系统

#### 3. 高级库管理
- [ ] 库文件格式解析 (.lib, .xml)
- [ ] 依赖关系检查
- [ ] 版本兼容性验证
- [ ] 自定义库路径UI

#### 4. 下载和调试
- [ ] 下载到设备实现
- [ ] 设备连接管理
- [ ] 在线调试支持

## API使用示例

### 1. 创建和配置项目

```cpp
// 创建新项目
ProjectController* controller = /* ... */;
controller->createNewProject("MyPLCProject", "/home/user/projects");

// 获取当前项目
auto project = model->currentProject();

// 设置项目信息
project->setAuthor("张三");
project->setDescription("PLC控制系统主程序");

// 配置编译选项
project->config().setTargetDevice("STVM-200");
project->config().setCompilerVersion("2.0.0");
project->config().setOptimizationEnabled(true);
project->config().setOptimizationLevel(2);
project->config().setDebugInfoEnabled(true);

// 配置通信参数
project->config().setCommunicationPort("COM3");
project->config().setBaudRate(115200);
project->config().setIpAddress("192.168.1.10");
project->config().setTcpPort(502);

// 保存项目
controller->saveProject();
```

### 2. 库管理

```cpp
// 获取所有可用库
auto availableLibs = controller->getAvailableLibraries();
for (const auto& lib : availableLibs) {
    qDebug() << "库:" << lib.name() 
             << "版本:" << lib.version()
             << "描述:" << lib.description();
}

// 添加库到项目
controller->addLibrary("Standard");   // 标准函数库
controller->addLibrary("Math");       // 数学函数库
controller->addLibrary("String");     // 字符串处理库
controller->addLibrary("Communication"); // 通信协议库

// 查看项目已引用的库
auto projectLibs = project->getAllLibraries();
qDebug() << "项目引用了" << projectLibs.size() << "个库";

// 移除库
controller->removeLibrary("String");

// 获取特定库信息
auto mathLib = project->getLibrary("Math");
if (!mathLib.name().isEmpty()) {
    qDebug() << "Math库路径:" << mathLib.path();
    qDebug() << "是否启用:" << mathLib.isEnabled();
}
```

### 3. POU管理

```cpp
// 添加Program
controller->addPOU(ProjectModel::Program, "MainProgram");

// 添加Function
controller->addPOU(ProjectModel::Function, "CalcAverage");

// 添加FunctionBlock
controller->addPOU(ProjectModel::FunctionBlock, "MotorController");

// 获取所有POU
auto allPOUs = project->getAllPOUs();
qDebug() << "总共" << allPOUs.size() << "个POU";

// 按类型获取POU
auto programs = project->getPOUsByType(POUType::Program);
auto functions = project->getPOUsByType(POUType::Function);
auto functionBlocks = project->getPOUsByType(POUType::FunctionBlock);

// 获取特定POU
auto mainPOU = project->getPOU("MainProgram");
if (mainPOU) {
    qDebug() << "POU类型:" << static_cast<int>(mainPOU->type());
    qDebug() << "代码行数:" << mainPOU->code().count('\n') + 1;
}

// 删除POU
controller->removePOU("OldFunction");
```

### 4. 项目统计

```cpp
// 获取项目统计信息
auto stats = controller->getProjectStatistics();

qDebug() << "=== 项目统计 ===";
qDebug() << "POU总数:" << stats.totalPOUs;
qDebug() << "  - Programs:" << stats.programs;
qDebug() << "  - Functions:" << stats.functions;
qDebug() << "  - FunctionBlocks:" << stats.functionBlocks;
qDebug() << "库数量:" << stats.libraries;
qDebug() << "代码总行数:" << stats.codeLines;

// 检查项目是否修改
if (project->isModified()) {
    qDebug() << "项目有未保存的更改";
    qDebug() << "最后修改时间:" << project->modifiedTime().toString();
}
```

### 5. 项目文件操作

```cpp
// 保存项目
if (controller->saveProject()) {
    qDebug() << "项目保存成功";
} else {
    qDebug() << "保存失败";
}

// 打开项目
if (controller->openProject("/path/to/project.nascode")) {
    qDebug() << "项目:" << project->name();
    qDebug() << "版本:" << project->version();
    qDebug() << "作者:" << project->author();
    qDebug() << "创建时间:" << project->createdTime().toString();
} else {
    qDebug() << "打开失败";
}

// 关闭项目
controller->closeProject();

// TODO: 导入导出 (待实现)
// project->exportToZip("/path/to/backup.zip");
// project->importFromZip("/path/to/backup.zip");
```

### 6. LibraryManager直接使用

```cpp
// 获取单例
auto& libManager = LibraryManager::instance();

// 添加自定义库路径
libManager.addLibraryPath("/usr/local/nascode/libs");
libManager.addLibraryPath("/home/user/my_libraries");

// 扫描库
libManager.scanLibraries();

// 获取标准库
auto stdLibs = libManager.getStandardLibraries();
for (const auto& lib : stdLibs) {
    qDebug() << "标准库:" << lib.name();
}

// 信号连接
QObject::connect(&libManager, &LibraryManager::librariesChanged,
    []() {
        qDebug() << "库列表已更新";
    });

QObject::connect(&libManager, &LibraryManager::libraryAdded,
    [](const QString& name) {
        qDebug() << "新库添加:" << name;
    });
```

## 项目文件格式

### JSON结构

```json
{
  "name": "MyPLCProject",
  "version": "1.0.0",
  "author": "张三",
  "description": "PLC控制系统主程序",
  "createdTime": "2024-01-15T10:30:00",
  "modifiedTime": "2024-01-15T15:45:00",
  "config": {
    "targetDevice": "STVM-200",
    "compilerVersion": "2.0.0",
    "optimizationEnabled": true,
    "optimizationLevel": 2,
    "debugInfoEnabled": true,
    "communicationPort": "COM3",
    "baudRate": 115200,
    "ipAddress": "192.168.1.10",
    "tcpPort": 502
  },
  "pous": [
    // TODO: POU序列化格式
  ],
  "libraries": [
    {
      "name": "Standard",
      "path": "",
      "version": "1.0.0",
      "enabled": true,
      "description": "IEC 61131-3 Standard Functions"
    },
    {
      "name": "Math",
      "path": "",
      "version": "1.0.0",
      "enabled": true,
      "description": "Mathematical functions"
    }
  ]
}
```

## 标准库列表

### 1. Standard (标准函数库)
- **描述**: IEC 61131-3标准定义的函数
- **包含**: 类型转换、位操作、比较等基础函数

### 2. String (字符串库)
- **描述**: 字符串处理函数
- **包含**: 拼接、查找、替换、转换等

### 3. Math (数学库)
- **描述**: 数学计算函数  
- **包含**: 三角函数、对数、指数、统计等

### 4. Communication (通信库)
- **描述**: 通信协议支持
- **包含**: Modbus, CANopen, EtherCAT等

## 目录结构

```
/home/user/nascode/
├── build/
│   └── NasCode (7.4MB)
├── src/
│   ├── models/
│   │   ├── Project.h/cpp            # 项目核心模型
│   │   ├── ProjectConfig.h/cpp      # 项目配置
│   │   ├── ProjectModel.h/cpp       # Qt Model/View
│   │   ├── LibraryManager.h/cpp     # 库管理器
│   │   └── POU.h/cpp                # POU定义
│   └── controllers/
│       └── ProjectController.h/cpp  # 项目控制器
├── docs/
│   ├── PROJECT_MANAGEMENT.md                 # 功能文档
│   ├── PROJECT_MANAGEMENT_IMPLEMENTATION.md  # 实现总结
│   └── PROJECT_MANAGEMENT_QUICKSTART.md      # 本文件
└── libraries/                                # 库目录
    ├── standard/
    ├── string/
    ├── math/
    └── communication/
```

## 编译说明

```bash
# 构建项目
cd /home/wei/nascode
cmake --build build

# 运行
./build/NasCode
```

## 代码规模

- **总代码行数**: 1525行 (项目管理相关文件)
- **新增文件**: 6个
- **修改文件**: 6个
- **可执行文件**: 7.4MB

## 相关文档

- [PROJECT_MANAGEMENT.md](PROJECT_MANAGEMENT.md) - 完整功能文档
- [PROJECT_MANAGEMENT_IMPLEMENTATION.md](PROJECT_MANAGEMENT_IMPLEMENTATION.md) - 实现细节
- [I18N_GUIDE.md](I18N_GUIDE.md) - 国际化指南
- [CODE_EDITOR_FEATURES.md](CODE_EDITOR_FEATURES.md) - 代码编辑器功能

## 常见问题

### Q: 如何添加自定义库?
A: 使用`LibraryManager::addLibraryPath()`添加库目录,然后调用`scanLibraries()`扫描。

### Q: 项目文件扩展名是什么?
A: 建议使用`.nascode`作为项目文件扩展名。

### Q: 如何备份项目?
A: 目前使用JSON格式保存,直接复制.nascode文件即可。ZIP导入导出功能待实现。

### Q: POU如何序列化?
A: POU序列化功能待实现,目前项目可以保存除POU代码外的所有信息。

### Q: 如何获取项目修改状态?
A: 使用`project->isModified()`检查,使用`project->modifiedTime()`获取最后修改时间。

---

**文档版本**: 1.0  
**更新时间**: 2024-10-28  
**维护者**: NasCode开发团队
