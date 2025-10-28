#pragma once

#include <QObject>
#include <memory>
#include "PluginManager.h"
#include "EventBus.h"
#include "I18nManager.h"

namespace nascode {
namespace core {

/**
 * @brief 应用程序核心类 - 单例模式
 * 负责应用程序的初始化、配置管理、插件管理等
 */
class Application : public QObject
{
    Q_OBJECT

public:
    static Application& getInstance();
    
    // 禁用拷贝和赋值
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /**
     * @brief 初始化应用程序
     */
    bool initialize();

    /**
     * @brief 关闭应用程序
     */
    void shutdown();

    /**
     * @brief 获取插件管理器
     */
    PluginManager* pluginManager() const { return m_pluginManager.get(); }

    /**
     * @brief 获取事件总线
     */
    EventBus* eventBus() const { return m_eventBus.get(); }

    /**
     * @brief 获取国际化管理器
     */
    I18nManager* i18nManager() const { return &I18nManager::getInstance(); }

signals:
    void initialized();
    void shutdownRequested();

private:
    Application();
    ~Application();

    void loadSettings();
    void saveSettings();
    void initializePlugins();

private:
    std::unique_ptr<PluginManager> m_pluginManager;
    std::unique_ptr<EventBus> m_eventBus;
    bool m_initialized;
};

} // namespace core
} // namespace nascode
