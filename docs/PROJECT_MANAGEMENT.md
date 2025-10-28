# 项目管理功能说明

## 功能概述

NasCode 的项目管理系统提供了完整的项目和库管理功能,包括:

### 1. 项目配置管理 (ProjectConfig)

#### 基本配置
- **目标设备**: 编译目标设备型号
- **编译器版本**: 编译器版本号
- **项目作者**: 项目创建者
- **项目描述**: 项目说明信息

#### 编译选项
- **优化开关**: 启用/禁用代码优化
- **优化级别**: 0-3级优化
- **调试信息**: 是否包含调试符号

#### 通信配置
- **串口配置**: COM端口、波特率
- **网络配置**: IP地址、TCP端口

### 2. 库管理 (LibraryManager)

#### 库引用系统
每个库包含以下信息:
- 库名称
- 库路径
- 版本号
- 启用状态
- 描述信息

#### 标准库
系统内置4个标准库:
1. **Standard**: IEC 61131-3标准函数
2. **String**: 字符串处理函数
3. **Math**: 数学计算函数
4. **Communication**: 通信协议库

#### 库路径管理
- 应用程序库目录: `{appDir}/libraries`
- 用户库目录: `~/.local/share/nascode/libraries`
- 支持自定义库路径

### 3. 项目统计

系统自动统计:
- 总POU数量
- 程序(Program)数量
- 函数(Function)数量
- 功能块(Function Block)数量
- 引用库数量
- 代码总行数

### 4. 项目序列化

#### JSON格式存储
```json
{
  "name": "项目名称",
  "version": "1.0.0",
  "author": "作者",
  "description": "项目描述",
  "createdTime": "2024-01-01T00:00:00",
  "modifiedTime": "2024-01-01T12:00:00",
  "config": {
    "targetDevice": "STVM-Default",
    "compilerVersion": "1.0.0",
    "optimizationEnabled": true,
    "optimizationLevel": 2,
    "debugInfoEnabled": true,
    "communicationPort": "COM1",
    "baudRate": 115200,
    "ipAddress": "192.168.1.100",
    "tcpPort": 502
  },
  "pous": [],
  "libraries": [
    {
      "name": "Standard",
      "path": "",
      "version": "1.0.0",
      "enabled": true,
      "description": "IEC 61131-3 Standard Functions"
    }
  ]
}
```

### 5. 项目控制器扩展

ProjectController 新增方法:
- `addLibrary(libraryName)`: 添加库引用
- `removeLibrary(libraryName)`: 移除库引用
- `getAvailableLibraries()`: 获取可用库列表
- `getProjectStatistics()`: 获取项目统计信息

## 使用示例

### 创建项目并添加库

```cpp
// 创建项目
controller->createNewProject("MyProject", "/path/to/project");

// 添加标准库
controller->addLibrary("Standard");
controller->addLibrary("Math");

// 获取项目统计
auto stats = controller->getProjectStatistics();
qDebug() << "Total POUs:" << stats.totalPOUs;
qDebug() << "Libraries:" << stats.libraries;

// 保存项目
controller->saveProject();
```

### 配置项目

```cpp
auto project = model->currentProject();
if (project) {
    // 设置目标设备
    project->config().setTargetDevice("STVM-100");
    
    // 配置通信
    project->config().setIpAddress("192.168.1.200");
    project->config().setTcpPort(502);
    
    // 启用优化
    project->config().setOptimizationEnabled(true);
    project->config().setOptimizationLevel(3);
}
```

### 管理库引用

```cpp
// 获取所有可用库
auto availableLibs = controller->getAvailableLibraries();
for (const auto& lib : availableLibs) {
    qDebug() << lib.name() << lib.version() << lib.description();
}

// 添加库
controller->addLibrary("Communication");

// 查看项目已引用的库
auto project = model->currentProject();
auto projectLibs = project->getAllLibraries();
```

## 文件结构

```
src/models/
├── ProjectConfig.h/cpp      # 项目配置类
├── LibraryManager.h/cpp     # 库管理器
├── Project.h/cpp            # 项目模型(已增强)
└── ProjectModel.h/cpp       # 项目树模型

src/controllers/
└── ProjectController.h/cpp  # 项目控制器(已扩展)
```

## 待实现功能

1. **ZIP导入导出**: `exportToZip()`和`importFromZip()`
2. **POU序列化**: 完整的POU JSON序列化
3. **库元数据解析**: 解析.lib和.xml库文件
4. **依赖检查**: 库版本兼容性检查
5. **项目模板**: 内置项目模板系统

## 技术细节

### 修改跟踪
- 任何修改都会更新`m_modifiedTime`
- `m_modified`标志指示是否有未保存更改
- 保存后自动清除modified标志

### 线程安全
- LibraryManager使用单例模式
- 信号槽机制确保UI线程安全

### 扩展性
- ProjectConfig易于添加新配置项
- LibraryReference支持自定义属性
- JSON格式便于版本升级
