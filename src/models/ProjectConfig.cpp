#include "ProjectConfig.h"

namespace nascode {
namespace models {

// ProjectConfig实现
ProjectConfig::ProjectConfig()
    : m_targetDevice("STVM-Default")
    , m_compilerVersion("1.0.0")
    , m_optimizationEnabled(true)
    , m_optimizationLevel(2)
    , m_debugInfoEnabled(true)
    , m_communicationPort("COM1")
    , m_baudRate(115200)
    , m_ipAddress("192.168.1.100")
    , m_tcpPort(502)
{
}

QVariantMap ProjectConfig::toVariant() const
{
    QVariantMap data;
    
    // 基本配置
    data["targetDevice"] = m_targetDevice;
    data["compilerVersion"] = m_compilerVersion;
    
    // 编译选项
    data["optimizationEnabled"] = m_optimizationEnabled;
    data["optimizationLevel"] = m_optimizationLevel;
    data["debugInfoEnabled"] = m_debugInfoEnabled;
    
    // 通信配置
    data["communicationPort"] = m_communicationPort;
    data["baudRate"] = m_baudRate;
    data["ipAddress"] = m_ipAddress;
    data["tcpPort"] = m_tcpPort;
    
    return data;
}

void ProjectConfig::fromVariant(const QVariantMap& data)
{
    // 基本配置
    m_targetDevice = data.value("targetDevice", "STVM-Default").toString();
    m_compilerVersion = data.value("compilerVersion", "1.0.0").toString();
    
    // 编译选项
    m_optimizationEnabled = data.value("optimizationEnabled", true).toBool();
    m_optimizationLevel = data.value("optimizationLevel", 2).toInt();
    m_debugInfoEnabled = data.value("debugInfoEnabled", true).toBool();
    
    // 通信配置
    m_communicationPort = data.value("communicationPort", "COM1").toString();
    m_baudRate = data.value("baudRate", 115200).toInt();
    m_ipAddress = data.value("ipAddress", "192.168.1.100").toString();
    m_tcpPort = data.value("tcpPort", 502).toInt();
}

// LibraryReference实现
QVariantMap LibraryReference::toVariant() const
{
    QVariantMap data;
    data["name"] = m_name;
    data["path"] = m_path;
    data["version"] = m_version;
    data["enabled"] = m_enabled;
    data["description"] = m_description;
    return data;
}

void LibraryReference::fromVariant(const QVariantMap& data)
{
    m_name = data.value("name").toString();
    m_path = data.value("path").toString();
    m_version = data.value("version", "1.0.0").toString();
    m_enabled = data.value("enabled", true).toBool();
    m_description = data.value("description").toString();
}

} // namespace models
} // namespace nascode
