# NasCode 插件开发指南

## 插件接口

所有插件必须实现 `IPlugin` 接口:

```cpp
class IPlugin
{
public:
    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual QString description() const = 0;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
};
```

## 示例插件

### 1. 创建插件类

```cpp
#include "core/IPlugin.h"

class ExamplePlugin : public QObject, public nascode::core::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.nascode.IPlugin/1.0")
    Q_INTERFACES(nascode::core::IPlugin)

public:
    QString name() const override { return "Example Plugin"; }
    QString version() const override { return "1.0.0"; }
    QString description() const override { return "An example plugin"; }
    
    bool initialize() override {
        // 初始化逻辑
        return true;
    }
    
    void shutdown() override {
        // 清理逻辑
    }
};
```

### 2. CMakeLists.txt

```cmake
add_library(ExamplePlugin SHARED
    ExamplePlugin.h
    ExamplePlugin.cpp
)

target_link_libraries(ExamplePlugin
    PRIVATE
        Qt6::Core
        NasCodeCore
)

install(TARGETS ExamplePlugin
    LIBRARY DESTINATION plugins
)
```

### 3. 访问核心服务

```cpp
bool ExamplePlugin::initialize()
{
    // 获取事件总线
    auto& app = Application::getInstance();
    auto* eventBus = app.eventBus();
    
    // 订阅事件
    eventBus->subscribe("project.opened", [](const QVariant& data) {
        qDebug() << "Project opened:" << data.toString();
    });
    
    return true;
}
```

## 插件类型

### 编辑器插件
扩展代码编辑功能

### 图形化编程插件
提供LD/FBD/SFC编辑器

### 通讯协议插件
增加新的通讯协议支持

### 调试工具插件
提供额外的调试和分析工具

### HMI插件
人机界面设计和运行时

## 插件API

### 事件系统
- `project.created`
- `project.opened`
- `project.closed`
- `pou.added`
- `pou.removed`
- `build.started`
- `build.finished`
- `debug.started`
- `debug.stopped`

### 服务接口
插件可以提供服务供其他插件使用
