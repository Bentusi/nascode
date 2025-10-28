#pragma once

#include <QString>
#include <QStringList>
#include <QVariantMap>

namespace nascode {
namespace models {

/**
 * @brief 项目配置类
 */
class ProjectConfig
{
public:
    ProjectConfig();

    // 基本配置
    QString targetDevice() const { return m_targetDevice; }
    void setTargetDevice(const QString& device) { m_targetDevice = device; }

    QString compilerVersion() const { return m_compilerVersion; }
    void setCompilerVersion(const QString& version) { m_compilerVersion = version; }

    // 编译选项
    bool optimizationEnabled() const { return m_optimizationEnabled; }
    void setOptimizationEnabled(bool enabled) { m_optimizationEnabled = enabled; }

    int optimizationLevel() const { return m_optimizationLevel; }
    void setOptimizationLevel(int level) { m_optimizationLevel = level; }

    bool debugInfoEnabled() const { return m_debugInfoEnabled; }
    void setDebugInfoEnabled(bool enabled) { m_debugInfoEnabled = enabled; }

    // 通信配置
    QString communicationPort() const { return m_communicationPort; }
    void setCommunicationPort(const QString& port) { m_communicationPort = port; }

    int baudRate() const { return m_baudRate; }
    void setBaudRate(int rate) { m_baudRate = rate; }

    QString ipAddress() const { return m_ipAddress; }
    void setIpAddress(const QString& ip) { m_ipAddress = ip; }

    int tcpPort() const { return m_tcpPort; }
    void setTcpPort(int port) { m_tcpPort = port; }

    // 序列化
    QVariantMap toVariant() const;
    void fromVariant(const QVariantMap& data);

private:
    // 基本配置
    QString m_targetDevice;
    QString m_compilerVersion;

    // 编译选项
    bool m_optimizationEnabled;
    int m_optimizationLevel;
    bool m_debugInfoEnabled;

    // 通信配置
    QString m_communicationPort;
    int m_baudRate;
    QString m_ipAddress;
    int m_tcpPort;
};

/**
 * @brief 库引用信息
 */
class LibraryReference
{
public:
    LibraryReference() = default;
    LibraryReference(const QString& name, const QString& path, const QString& version)
        : m_name(name), m_path(path), m_version(version), m_enabled(true) {}

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    QString path() const { return m_path; }
    void setPath(const QString& path) { m_path = path; }

    QString version() const { return m_version; }
    void setVersion(const QString& version) { m_version = version; }

    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }

    QString description() const { return m_description; }
    void setDescription(const QString& desc) { m_description = desc; }

    // 序列化
    QVariantMap toVariant() const;
    void fromVariant(const QVariantMap& data);

private:
    QString m_name;
    QString m_path;
    QString m_version;
    bool m_enabled;
    QString m_description;
};

} // namespace models
} // namespace nascode
