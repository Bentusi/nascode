#include "Project.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <spdlog/spdlog.h>

namespace nascode {
namespace models {

Project::Project()
    : m_version("1.0.0")
    , m_modified(false)
{
}

Project::~Project()
{
}

bool Project::addPOU(std::shared_ptr<POU> pou)
{
    if (!pou) {
        return false;
    }

    QString name = pou->name();
    if (m_pous.contains(name)) {
        spdlog::warn("POU already exists: {}", name.toStdString());
        return false;
    }

    m_pous[name] = pou;
    m_modified = true;
    return true;
}

bool Project::removePOU(const QString& name)
{
    if (!m_pous.contains(name)) {
        return false;
    }

    m_pous.remove(name);
    m_modified = true;
    return true;
}

std::shared_ptr<POU> Project::getPOU(const QString& name) const
{
    return m_pous.value(name, nullptr);
}

QList<std::shared_ptr<POU>> Project::getAllPOUs() const
{
    return m_pous.values();
}

bool Project::addLibrary(const QString& libraryPath)
{
    if (m_libraries.contains(libraryPath)) {
        return false;
    }

    m_libraries.append(libraryPath);
    m_modified = true;
    return true;
}

bool Project::removeLibrary(const QString& libraryName)
{
    bool removed = m_libraries.removeOne(libraryName);
    if (removed) {
        m_modified = true;
    }
    return removed;
}

bool Project::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        spdlog::error("Failed to open project file: {}", filePath.toStdString());
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isNull() || !doc.isObject()) {
        spdlog::error("Invalid project file format");
        return false;
    }

    QJsonObject root = doc.object();
    m_name = root["name"].toString();
    m_version = root["version"].toString();
    
    // TODO: 加载POUs和库
    
    m_path = filePath;
    m_modified = false;
    
    spdlog::info("Project loaded: {}", m_name.toStdString());
    return true;
}

bool Project::saveToFile(const QString& filePath)
{
    QJsonObject root;
    root["name"] = m_name;
    root["version"] = m_version;
    
    // TODO: 保存POUs和库
    
    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        spdlog::error("Failed to save project file: {}", filePath.toStdString());
        return false;
    }

    file.write(doc.toJson());
    m_modified = false;
    
    spdlog::info("Project saved: {}", filePath.toStdString());
    return true;
}

} // namespace models
} // namespace nascode
