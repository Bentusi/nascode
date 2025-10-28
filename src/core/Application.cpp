#include "Application.h"
#include <QSettings>
#include <spdlog/spdlog.h>

namespace nascode {
namespace core {

Application& Application::getInstance()
{
    static Application instance;
    return instance;
}

Application::Application()
    : m_pluginManager(std::make_unique<PluginManager>())
    , m_eventBus(std::make_unique<EventBus>())
    , m_initialized(false)
{
}

Application::~Application()
{
    shutdown();
}

bool Application::initialize()
{
    if (m_initialized) {
        return true;
    }

    spdlog::info("Initializing NasCode Application...");

    // 加载配置
    loadSettings();

    // 初始化国际化系统
    I18nManager::getInstance().initialize();

    // 初始化事件总线
    // m_eventBus已在构造函数中创建

    // 初始化插件系统
    initializePlugins();

    m_initialized = true;
    emit initialized();

    spdlog::info("NasCode Application initialized successfully");
    return true;
}

void Application::shutdown()
{
    if (!m_initialized) {
        return;
    }

    spdlog::info("Shutting down NasCode Application...");

    emit shutdownRequested();

    // 保存配置
    saveSettings();

    // 卸载插件
    if (m_pluginManager) {
        m_pluginManager->unloadAllPlugins();
    }

    m_initialized = false;
}

void Application::loadSettings()
{
    QSettings settings("NasCode", "NasCode");
    // 加载应用配置
    spdlog::debug("Loading application settings...");
}

void Application::saveSettings()
{
    QSettings settings("NasCode", "NasCode");
    // 保存应用配置
    spdlog::debug("Saving application settings...");
}

void Application::initializePlugins()
{
    spdlog::info("Initializing plugin system...");
    m_pluginManager->loadPluginsFromDirectory("plugins");
}

} // namespace core
} // namespace nascode
