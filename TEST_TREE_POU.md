# 树形结构和POU源文件保存功能测试说明

## 已完成的功能

### 1. POU序列化/反序列化
- ✅ 添加了 `POU::toVariant()` 方法 - 将POU对象序列化为QVariantMap
- ✅ 添加了 `POU::fromVariant()` 方法 - 从QVariantMap反序列化POU对象
- ✅ 添加了 `POU::generateSTCode()` 方法 - 生成完整的ST语言代码文本

#### 序列化内容包括：
- POU名称
- POU类型（Program/Function/FunctionBlock）
- 代码主体
- 注释
- 变量列表（包含7种变量段）

### 2. 项目文件保存/加载
- ✅ 完善了 `Project::saveToFile()` - 将所有POU序列化到JSON
- ✅ 完善了 `Project::loadFromFile()` - 从JSON反序列化POU对象
- ✅ JSON文件结构：包含project配置、libraries和pous数组

### 3. 树形结构显示
- ✅ 实现了 `ProjectModel::setupModelData()` - 构建完整的项目树结构
- ✅ 树形结构包括：
  - 项目根节点
  - Programs文件夹（显示所有Program类型的POU）
  - Functions文件夹（显示所有Function类型的POU）
  - Function Blocks文件夹（显示所有FunctionBlock类型的POU）
  - Libraries文件夹（显示所有库引用）

### 4. POU管理功能
- ✅ `ProjectModel::addPOU()` - 添加POU并刷新树
- ✅ `ProjectModel::removePOU()` - 删除POU并刷新树
- ✅ `ProjectModel::getPOU()` - 根据树节点获取POU对象
- ✅ 添加/删除POU时自动刷新树形结构

## 测试步骤

### 测试1: 创建项目并查看树结构
1. 启动NasCode应用程序
2. 点击"新建工程"
3. 输入工程名称（如：TestProject）
4. 选择保存位置
5. **期望结果**：左侧树形视图显示项目结构：
   ```
   TestProject (Project)
   ├─ Programs (Folder)
   ├─ Functions (Folder)
   ├─ Function Blocks (Folder)
   └─ Libraries (Folder)
   ```

### 测试2: 添加POU（需要后续实现UI）
当前版本可通过代码测试：
```cpp
// 在ProjectController中添加测试代码
auto pou = std::make_shared<Program>("MainProgram");
pou->setCode("// Main program code");
m_model->addPOU(ProjectModel::ProgramItem, "MainProgram");
```

### 测试3: 保存和加载项目
1. 创建项目
2. 保存项目（Ctrl+S）
3. 检查生成的JSON文件
4. 关闭项目
5. 重新打开项目
6. **期望结果**：项目树结构完整恢复

### 测试4: JSON文件格式
打开保存的项目文件（.nascode），应该看到如下结构：
```json
{
    "project": {
        "name": "TestProject",
        "version": "1.0.0",
        "author": "",
        "description": ""
    },
    "libraries": [],
    "pous": [
        {
            "name": "MainProgram",
            "type": 0,
            "code": "// Main program code",
            "comment": "",
            "variables": []
        }
    ]
}
```

## 代码生成功能

POU对象可以生成完整的ST语言代码：
```cpp
auto pou = std::make_shared<Program>("MainProgram");
pou->setCode("x := x + 1;");

Variable var;
var.name = "x";
var.dataType = "INT";
var.initialValue = "0";
var.section = Variable::VAR;
pou->addVariable(var);

QString stCode = pou->generateSTCode();
```

生成的ST代码：
```
PROGRAM MainProgram
VAR
    x : INT := 0;
END_VAR

x := x + 1;

END_PROGRAM
```

## 后续开发建议

### 短期任务：
1. **添加右键菜单** - 在树形视图中添加右键菜单，支持：
   - 新建Program/Function/FunctionBlock
   - 删除POU
   - 重命名POU
   
2. **双击打开POU编辑器** - 双击树节点打开代码编辑器

3. **POU属性对话框** - 创建对话框编辑POU属性：
   - 名称
   - 类型
   - 注释
   - 变量列表

### 中期任务：
1. **变量编辑器** - 创建专门的变量编辑界面
2. **代码语法高亮** - 集成之前实现的CodeEditor
3. **代码自动完成** - 基于变量列表提供智能提示

### 长期任务：
1. **调试支持** - POU断点、变量监视
2. **版本控制** - POU历史版本管理
3. **导出/导入** - 支持导出单个POU文件

## 技术亮点

1. **完整的序列化系统**
   - 使用Qt的QVariant系统
   - 支持复杂的嵌套数据结构
   - 易于扩展新的POU属性

2. **自动树刷新机制**
   - beginResetModel/endResetModel保证UI同步
   - setupModelData自动重建整个树结构

3. **类型安全的POU管理**
   - 使用智能指针shared_ptr
   - 明确的类型枚举（POUType）
   - 类型安全的变量段（Variable::Section）

## 文件清单

修改/新增的文件：
- `src/models/POU.h` - 添加序列化接口
- `src/models/POU.cpp` - 实现toVariant/fromVariant/generateSTCode
- `src/models/Project.cpp` - 完善POU保存/加载
- `src/models/ProjectModel.h` - 添加getPOU方法
- `src/models/ProjectModel.cpp` - 实现完整的树构建和POU管理

代码统计：
- POU序列化：约140行
- 树形结构构建：约90行
- POU管理功能：约60行
- 总计：约290行新增代码
