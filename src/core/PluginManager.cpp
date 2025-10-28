#include "PluginManager.h"
#include <QDir>
#include <QFileInfo>
#include <spdlog/spdlog.h>

namespace nascode {
namespace core {

PluginManager::PluginManager(QObject* parent)
    : QObject(parent)
{
}

PluginManager::~PluginManager()
{
    unloadAllPlugins();
}

void PluginManager::loadPluginsFromDirectory(const QString& directory)
{
    QDir pluginsDir(directory);
    if (!pluginsDir.exists()) {
        spdlog::warn("Plugins directory does not exist: {}", directory.toStdString());
        return;
    }

    const QStringList filters = {
#ifdef Q_OS_WIN
        "*.dll"
#elif defined(Q_OS_MAC)
        "*.dylib"
#else
        "*.so"
#endif
    };

    pluginsDir.setNameFilters(filters);
    const QFileInfoList files = pluginsDir.entryInfoList(QDir::Files);

    for (const QFileInfo& file : files) {
        loadPlugin(file.absoluteFilePath());
    }
}

bool PluginManager::loadPlugin(const QString& filePath)
{
    auto* loader = new QPluginLoader(filePath);
    QObject* pluginObject = loader->instance();

    if (!pluginObject) {
        spdlog::error("Failed to load plugin: {} - {}", 
            filePath.toStdString(), 
            loader->errorString().toStdString());
        delete loader;
        return false;
    }

    IPlugin* plugin = qobject_cast<IPlugin*>(pluginObject);
    if (!plugin) {
        spdlog::error("Plugin does not implement IPlugin interface: {}", filePath.toStdString());
        loader->unload();
        delete loader;
        return false;
    }

    QString pluginName = plugin->name();
    if (m_plugins.contains(pluginName)) {
        spdlog::warn("Plugin already loaded: {}", pluginName.toStdString());
        loader->unload();
        delete loader;
        return false;
    }

    if (!plugin->initialize()) {
        spdlog::error("Failed to initialize plugin: {}", pluginName.toStdString());
        loader->unload();
        delete loader;
        return false;
    }

    PluginInfo* info = new PluginInfo();
    info->loader = loader;
    info->interface = plugin;
    info->name = pluginName;
    info->version = plugin->version();
    info->enabled = true;

    m_plugins[pluginName] = info;

    spdlog::info("Plugin loaded: {} v{}", pluginName.toStdString(), plugin->version().toStdString());
    emit pluginLoaded(pluginName);

    return true;
}

bool PluginManager::unloadPlugin(const QString& pluginName)
{
    if (!m_plugins.contains(pluginName)) {
        return false;
    }

    PluginInfo* info = m_plugins[pluginName];
    info->interface->shutdown();
    info->loader->unload();
    
    delete info->loader;
    delete info;
    
    m_plugins.remove(pluginName);
    
    spdlog::info("Plugin unloaded: {}", pluginName.toStdString());
    emit pluginUnloaded(pluginName);

    return true;
}

void PluginManager::unloadAllPlugins()
{
    QStringList pluginNames = getPluginNames();
    for (const QString& name : pluginNames) {
        unloadPlugin(name);
    }
}

IPlugin* PluginManager::getPlugin(const QString& pluginName) const
{
    if (m_plugins.contains(pluginName)) {
        return m_plugins[pluginName]->interface;
    }
    return nullptr;
}

QStringList PluginManager::getPluginNames() const
{
    return m_plugins.keys();
}

} // namespace core
} // namespace nascode
