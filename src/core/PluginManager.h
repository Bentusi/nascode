#pragma once

#include <QObject>
#include <QPluginLoader>
#include <QMap>
#include <QString>
#include <memory>
#include "IPlugin.h"

namespace nascode {
namespace core {

/**
 * @brief 插件管理器
 */
class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject* parent = nullptr);
    ~PluginManager();

    /**
     * @brief 从目录加载插件
     */
    void loadPluginsFromDirectory(const QString& directory);

    /**
     * @brief 加载单个插件
     */
    bool loadPlugin(const QString& filePath);

    /**
     * @brief 卸载插件
     */
    bool unloadPlugin(const QString& pluginName);

    /**
     * @brief 卸载所有插件
     */
    void unloadAllPlugins();

    /**
     * @brief 获取插件
     */
    IPlugin* getPlugin(const QString& pluginName) const;

    /**
     * @brief 获取所有插件名称
     */
    QStringList getPluginNames() const;

signals:
    void pluginLoaded(const QString& pluginName);
    void pluginUnloaded(const QString& pluginName);

private:
    struct PluginInfo {
        QPluginLoader* loader;
        IPlugin* interface;
        QString name;
        QString version;
        bool enabled;
    };

    QMap<QString, PluginInfo*> m_plugins;
};

} // namespace core
} // namespace nascode
