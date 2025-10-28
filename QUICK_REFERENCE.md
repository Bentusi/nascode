# NasCode - 快速开发参考

## 项目当前状态

### ✅ 已完成的核心功能

1. **国际化系统** (15文件, 1359行)
   - 中英文双语支持
   - 47个翻译条目
   - 动态语言切换

2. **代码编辑器** (5文件, 1170行)
   - ST语言语法高亮
   - 代码自动完成
   - 代码片段
   - 行号显示
   - 括号匹配
   - 代码折叠

3. **项目管理** (12文件, 1525行)
   - 项目配置管理
   - 库引用管理
   - JSON序列化/反序列化
   - 文件监视

4. **UI集成** (232行)
   - MVC架构连接
   - 新建/打开/保存/关闭项目对话框
   - 菜单和工具栏完整

5. **POU管理** (290行，本次新增)
   - POU序列化/反序列化
   - 树形结构显示
   - 添加/删除POU
   - ST代码生成

### 总代码量：约4576行

## 快速API参考

### 1. 创建POU

```cpp
// 创建Program
auto program = std::make_shared<Program>("MainProgram");
program->setComment("主程序");
program->setCode("// Your code here");

// 添加变量
Variable var;
var.name = "Counter";
var.dataType = "INT";
var.initialValue = "0";
var.comment = "计数器";
var.section = Variable::VAR;
program->addVariable(var);

// 添加到项目
project->addPOU(program);
```

### 2. 项目管理

```cpp
// 创建项目
ProjectModel* model = new ProjectModel();
model->createProject("MyProject", "/path/to/project.nascode");

// 打开项目
model->openProject("/path/to/project.nascode");

// 保存项目
model->saveProject();

// 添加POU到项目
model->addPOU(ProjectModel::ProgramItem, "NewProgram");

// 获取POU
auto pou = model->getPOU(treeIndex);

// 删除POU
model->removePOU(treeIndex);
```

### 3. 序列化

```cpp
// POU转JSON
QVariantMap data = pou->toVariant();
QJsonObject json = QJsonObject::fromVariantMap(data);
QJsonDocument doc(json);
QString jsonString = doc.toJson();

// JSON转POU
QJsonDocument doc = QJsonDocument::fromJson(jsonData);
QVariantMap data = doc.object().toVariantMap();
auto pou = std::make_shared<Program>("Name");
pou->fromVariant(data);
```

### 4. 生成ST代码

```cpp
QString stCode = pou->generateSTCode();
// 输出标准ST格式代码，包含变量声明和代码主体
```

### 5. 树形视图集成

```cpp
// 在MainWindow中
m_projectTree = new QTreeView(this);
m_projectModel = new ProjectModel(this);
m_projectTree->setModel(m_projectModel);

// 连接信号
connect(m_projectModel, &ProjectModel::projectOpened,
        this, &MainWindow::onProjectOpened);
connect(m_projectModel, &ProjectModel::pouAdded,
        this, &MainWindow::onPouAdded);
```

## 类层次结构

```
POU (基类)
├─ Program
├─ Function
└─ FunctionBlock

ProjectModel (QAbstractItemModel)
└─ TreeItem (内部类)
    ├─ ProjectRoot
    ├─ Folders (Programs/Functions/Function Blocks/Libraries)
    └─ Items (具体的POU或Library)

Project
├─ ProjectConfig
├─ QMap<QString, shared_ptr<POU>> m_pous
└─ QList<LibraryReference> m_libraries
```

## 变量段类型

```cpp
enum Section {
    VAR = 0,           // 内部变量
    VAR_INPUT = 1,     // 输入变量
    VAR_OUTPUT = 2,    // 输出变量
    VAR_IN_OUT = 3,    // 输入输出变量
    VAR_TEMP = 4,      // 临时变量
    VAR_EXTERNAL = 5,  // 外部变量
    VAR_GLOBAL = 6     // 全局变量
};
```

## JSON文件格式

```json
{
    "name": "项目名称",
    "version": "1.0.0",
    "author": "作者",
    "description": "描述",
    "createdTime": "2025-10-28T17:15:31",
    "modifiedTime": "2025-10-28T17:15:31",
    "config": {
        "targetDevice": "STVM-Default",
        "optimizationLevel": 2,
        ...
    },
    "libraries": [
        {
            "name": "StandardLib",
            "version": "1.0.0",
            "path": "/path/to/lib"
        }
    ],
    "pous": [
        {
            "name": "MainProgram",
            "type": 0,  // 0=Program, 1=Function, 2=FunctionBlock
            "code": "// ST代码",
            "comment": "注释",
            "variables": [
                {
                    "name": "Counter",
                    "dataType": "INT",
                    "initialValue": "0",
                    "comment": "计数器",
                    "section": 0  // 变量段
                }
            ]
        }
    ]
}
```

## 常用文件路径

```
/home/wei/nascode/
├── src/
│   ├── models/
│   │   ├── POU.h/cpp           # POU类
│   │   ├── Project.h/cpp       # 项目类
│   │   └── ProjectModel.h/cpp  # 树形模型
│   ├── views/
│   │   └── MainWindow.h/cpp    # 主窗口
│   ├── controllers/
│   │   └── ProjectController.h/cpp
│   └── core/
│       ├── CodeEditor.h/cpp    # 代码编辑器
│       └── I18nManager.h/cpp   # 国际化
├── examples/
│   └── test_pou_serialization.cpp  # 测试示例
├── translations/
│   ├── nascode_zh_CN.ts/qm    # 中文翻译
│   └── nascode_en_US.ts/qm    # 英文翻译
└── build/
    ├── NasCode                # 主程序
    └── examples/
        └── test_pou_serialization  # 测试程序
```

## 编译命令

```bash
# 完整编译
cd /home/wei/nascode
cmake --build build

# 只编译主程序
cmake --build build --target NasCode

# 编译测试程序
cmake --build build --target test_pou_serialization

# 运行主程序
./build/NasCode

# 运行测试程序
./build/examples/test_pou_serialization
```

## 调试技巧

### 1. 查看spdlog日志
```cpp
spdlog::info("Project saved: {}", project->name().toStdString());
spdlog::warn("POU not found: {}", pouName.toStdString());
spdlog::error("Failed to load: {}", filePath.toStdString());
```

### 2. Qt调试输出
```cpp
qDebug() << "POU count:" << pous.size();
qDebug() << "JSON:" << doc.toJson();
```

### 3. 断点位置建议
- `ProjectModel::setupModelData()` - 树构建
- `POU::toVariant()` - 序列化
- `Project::saveToFile()` - 文件保存
- `MainWindow::onNewProject()` - UI交互

## 下一步开发任务

### 优先级1（必需）：
- [ ] 右键菜单 - 新建/删除POU
- [ ] 双击打开 - POU编辑器集成
- [ ] POU属性对话框

### 优先级2（重要）：
- [ ] 变量编辑器
- [ ] 代码编辑器集成到MDI
- [ ] 编译错误显示

### 优先级3（增强）：
- [ ] 拖拽重排
- [ ] 搜索功能
- [ ] 导出/导入POU

## 常见问题

### Q: 如何添加新的POU类型？
A: 
1. 在`POUType`枚举中添加新类型
2. 创建新的POU子类
3. 在`ProjectModel::addPOU()`中添加处理
4. 在`setupModelData()`中添加文件夹

### Q: 如何扩展变量属性？
A:
1. 在`Variable`结构体中添加新字段
2. 更新`POU::toVariant()`序列化
3. 更新`POU::fromVariant()`反序列化
4. 更新`generateSTCode()`代码生成

### Q: 如何添加新的项目配置？
A:
1. 在`ProjectConfig`类中添加属性
2. 更新`ProjectConfig::toJson()`
3. 更新`ProjectConfig::fromJson()`

## 性能优化建议

1. **大型项目加载**
   - 使用异步加载
   - 延迟树构建
   - 按需加载POU代码

2. **序列化优化**
   - 考虑使用二进制格式（可选）
   - 增量保存（只保存修改的POU）
   - 压缩JSON文件

3. **树刷新优化**
   - 使用dataChanged信号而非resetModel
   - 实现增量更新
   - 缓存树节点

## 测试清单

- [x] POU序列化正确性
- [x] 项目文件保存/加载
- [x] 树形结构显示
- [x] ST代码生成格式
- [x] 变量序列化完整性
- [ ] 大量POU性能测试
- [ ] 并发访问安全性
- [ ] 文件损坏恢复

## 资源链接

- IEC-61131-3标准文档
- Qt Model/View编程指南
- spdlog文档
- CMake官方文档
