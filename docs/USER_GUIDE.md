# NasCode 项目管理 - 用户指南

## 快速开始

### 启动应用

```bash
cd /home/wei/nascode
./build/NasCode
```

应用启动后,您将看到主窗口界面。

## 创建新项目

### 方法1: 菜单栏
1. 点击 `File -> New Project...`
2. 或使用快捷键 `Ctrl+N`

### 方法2: 工具栏
1. 点击工具栏的 📄 新建图标

### 创建流程
```
┌─────────────────────────────────┐
│  New Project                    │
│  ───────────────────────────   │
│  Project name:                  │
│  [MyPLCProject        ]  [确定] │
│                          [取消] │
└─────────────────────────────────┘
         ↓
┌─────────────────────────────────┐
│  Create New Project             │
│  ───────────────────────────   │
│  保存位置:                      │
│  /home/user/                    │
│                                 │
│  文件名: MyPLCProject.nascode   │
│                [保存]   [取消]  │
└─────────────────────────────────┘
         ↓
    项目创建成功!
```

### 项目文件
创建的项目文件是JSON格式:
```
/home/user/MyPLCProject.nascode
```

## 打开已有项目

### 操作步骤
1. 点击 `File -> Open Project...` 或 `Ctrl+O`
2. 浏览到项目文件位置
3. 选择 `.nascode` 文件
4. 点击 "打开"

### 文件类型过滤
- **NasCode Project Files (*.nascode)** - 推荐
- **All Files (*)** - 显示所有文件

### 打开后
- 项目树显示项目结构
- 状态栏显示: "Project opened: 项目名"

## 项目结构

打开项目后,左侧项目树显示:

```
📁 MyPLCProject
├── 📂 Programs
│   └── 📄 MainProgram
├── 📂 Functions
│   ├── 📄 CalcSum
│   └── 📄 CalcAverage
├── 📂 Function Blocks
│   └── 📄 MotorController
└── 📚 Libraries
    ├── ✓ Standard (1.0.0)
    ├── ✓ Math (1.0.0)
    └── ✓ String (1.0.0)
```

## 保存项目

### 保存当前项目
- 点击 `File -> Save` 或 `Ctrl+S`
- 状态栏显示: "Project saved"

### 自动保存
- 项目修改后,modified标志自动设置
- modifiedTime自动更新

### 保存内容
JSON文件包含:
- ✅ 项目基本信息 (名称、版本、作者、描述)
- ✅ 项目配置 (设备、编译选项、通信参数)
- ✅ 库引用列表
- ✅ 创建/修改时间
- ⏳ POUs (待实现序列化)

## 关闭项目

1. 点击 `File -> Close Project`
2. 项目树清空
3. 编辑器标签页关闭

⚠️ **注意**: 关闭前请确保已保存更改

## 项目配置

项目创建后的默认配置:

### 基本信息
- **版本**: 1.0.0
- **作者**: 当前用户名
- **创建时间**: 自动记录

### 编译配置
- **目标设备**: STVM-Default
- **编译器版本**: 1.0.0
- **优化**: 启用,级别2
- **调试信息**: 启用

### 通信配置
- **串口**: COM1, 115200
- **网络**: 192.168.1.100:502

## 库管理

### 标准库
项目默认可用的标准库:

#### 1. Standard 库
- **功能**: IEC 61131-3标准函数
- **包含**: 类型转换、位操作、比较等
- **版本**: 1.0.0

#### 2. String 库
- **功能**: 字符串处理
- **包含**: 拼接、查找、替换、转换
- **版本**: 1.0.0

#### 3. Math 库
- **功能**: 数学计算
- **包含**: 三角函数、对数、指数
- **版本**: 1.0.0

#### 4. Communication 库
- **功能**: 通信协议
- **包含**: Modbus, CANopen等
- **版本**: 1.0.0

### 添加库到项目
(通过代码或未来的UI对话框)
```cpp
controller->addLibrary("Standard");
controller->addLibrary("Math");
```

## 状态栏信息

状态栏显示实时信息:

### 操作消息
- `Ready` - 就绪
- `Project created: 项目名` - 创建成功
- `Project opened: 项目名` - 打开成功
- `Project saved` - 保存成功
- `Project closed` - 关闭成功

### 错误提示
错误会以对话框形式显示:
- `Failed to create project` - 创建失败
- `Failed to open project: 路径` - 打开失败
- `Failed to save project` - 保存失败
- `Project controller not initialized` - 控制器未初始化

## 快捷键

| 功能 | 快捷键 |
|------|--------|
| 新建项目 | Ctrl+N |
| 打开项目 | Ctrl+O |
| 保存项目 | Ctrl+S |
| 退出应用 | Ctrl+Q |
| 撤销 | Ctrl+Z |
| 重做 | Ctrl+Y |
| 剪切 | Ctrl+X |
| 复制 | Ctrl+C |
| 粘贴 | Ctrl+V |

## 项目文件示例

创建项目后生成的JSON文件:

```json
{
  "name": "MyPLCProject",
  "version": "1.0.0",
  "author": "wei",
  "description": "",
  "createdTime": "2025-10-28T16:54:00",
  "modifiedTime": "2025-10-28T16:54:00",
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
  "libraries": []
}
```

## 工作流程示例

### 场景1: 创建新的PLC项目

```
1. 启动NasCode
   ./build/NasCode

2. 创建项目
   File -> New Project
   输入: "ConveyorControl"
   保存: ~/projects/ConveyorControl.nascode

3. 添加标准库 (通过代码)
   controller->addLibrary("Standard");
   controller->addLibrary("Math");

4. 创建POUs (未来通过UI)
   - 右键 Programs -> New Program -> "MainControl"
   - 右键 Functions -> New Function -> "CalcSpeed"
   - 右键 Function Blocks -> New FB -> "MotorControl"

5. 编写代码
   (在代码编辑器中编写ST代码)

6. 保存项目
   Ctrl+S

7. 编译
   Build -> Build Project

8. 下载到设备
   Debug -> Download
```

### 场景2: 打开并修改已有项目

```
1. 打开项目
   File -> Open Project
   选择: ~/projects/ConveyorControl.nascode

2. 查看项目树
   (自动显示所有POUs和库)

3. 编辑代码
   双击 MainControl -> 在编辑器中修改

4. 添加新库
   (通过代码或未来的库管理器)

5. 保存更改
   Ctrl+S

6. 查看修改时间
   (项目文件中的modifiedTime自动更新)
```

## 常见问题

### Q1: 项目文件保存在哪里?
A: 您在创建项目时选择的位置,推荐使用 `~/projects/` 目录。

### Q2: 可以重命名项目吗?
A: 目前需要手动修改JSON文件中的name字段,未来会添加UI功能。

### Q3: 如何备份项目?
A: 直接复制 `.nascode` 文件即可。未来会支持导出为ZIP。

### Q4: 支持多项目同时打开吗?
A: 目前只支持单项目,未来版本会支持多项目工作区。

### Q5: 项目文件可以手动编辑吗?
A: 可以,使用任何文本编辑器编辑JSON文件,但要注意格式正确性。

### Q6: 如何查看项目统计信息?
A: 目前通过代码调用 `getProjectStatistics()`,未来会添加UI显示。

## 界面布局

```
┌─────────────────────────────────────────────────────────┐
│ NasCode - IEC-61131 ST Programming Environment         │
├─────────────────────────────────────────────────────────┤
│ File  Edit  Build  Debug  Tools  Help                  │
├─────────────────────────────────────────────────────────┤
│ [📄] [📁] [💾] │ [↶] [↷] │ [✂] [📋] [📄] │ [🔨] │ [⬇] [▶]│
├───────────────┬─────────────────────────────────────────┤
│ 📁 MyProject  │  MainProgram.st    [×]                  │
│ ├─Programs    │ ┌─────────────────────────────────────┐ │
│ │ └─MainPro.. │ │ PROGRAM MainProgram                 │ │
│ ├─Functions   │ │ VAR                                 │ │
│ │ ├─CalcSum   │ │   counter : INT;                    │ │
│ │ └─CalcAvg   │ │   total : REAL;                     │ │
│ ├─FuncBlocks  │ │ END_VAR                             │ │
│ │ └─MotorCtl  │ │                                     │ │
│ └─Libraries   │ │ (* Main control logic *)            │ │
│   ├─✓Standard │ │ counter := counter + 1;             │ │
│   ├─✓Math     │ │                                     │ │
│   └─✓String   │ │ END_PROGRAM                         │ │
│               │ └─────────────────────────────────────┘ │
├───────────────┴─────────────────────────────────────────┤
│ Output                                                  │
│ Build started...                                        │
│ Compiling MainProgram.st...                             │
│ Build successful. 0 errors, 0 warnings.                 │
└─────────────────────────────────────────────────────────┘
│ Ready                                        Line 1 Col 1│
└─────────────────────────────────────────────────────────┘
```

## 下一步

### 学习资源
- **代码编辑器功能**: [CODE_EDITOR_FEATURES.md](CODE_EDITOR_FEATURES.md)
- **项目管理API**: [PROJECT_MANAGEMENT_QUICKSTART.md](PROJECT_MANAGEMENT_QUICKSTART.md)
- **国际化支持**: [I18N_GUIDE.md](I18N_GUIDE.md)

### 推荐工作流
1. 熟悉UI界面
2. 创建第一个测试项目
3. 学习ST语言语法
4. 编写简单程序
5. 使用代码补全功能
6. 编译和调试

---

**开始您的IEC-61131-3编程之旅!** 🚀
