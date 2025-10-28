#pragma once

#include <QString>
#include <QtPlugin>

namespace nascode {
namespace core {

/**
 * @brief 插件接口
 */
class IPlugin
{
public:
    virtual ~IPlugin() = default;

    /**
     * @brief 获取插件名称
     */
    virtual QString name() const = 0;

    /**
     * @brief 获取插件版本
     */
    virtual QString version() const = 0;

    /**
     * @brief 获取插件描述
     */
    virtual QString description() const = 0;

    /**
     * @brief 初始化插件
     */
    virtual bool initialize() = 0;

    /**
     * @brief 关闭插件
     */
    virtual void shutdown() = 0;
};

} // namespace core
} // namespace nascode

Q_DECLARE_INTERFACE(nascode::core::IPlugin, "com.nascode.IPlugin/1.0")
