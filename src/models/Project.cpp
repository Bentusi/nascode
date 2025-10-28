#include "Project.h"
#include <spdlog/spdlog.h>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace nascode {
namespace models {

Project::Project()
    : m_version("1.0.0")
    , m_author(qgetenv("USER"))
    , m_createdTime(QDateTime::currentDateTime())
    , m_modifiedTime(QDateTime::currentDateTime())
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
    setModified(true);
    return true;
}

bool Project::removePOU(const QString& name)
{
    if (!m_pous.contains(name)) {
        return false;
    }

    m_pous.remove(name);
    setModified(true);
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

QList<std::shared_ptr<POU>> Project::getPOUsByType(POUType type) const
{
    QList<std::shared_ptr<POU>> result;
    for (const auto& pou : m_pous.values()) {
        if (pou->type() == type) {
            result.append(pou);
        }
    }
    return result;
}

bool Project::addLibrary(const LibraryReference& library)
{
    if (m_libraries.contains(library.name())) {
        spdlog::warn("Library already exists: {}", library.name().toStdString());
        return false;
    }

    m_libraries[library.name()] = library;
    setModified(true);
    return true;
}

bool Project::removeLibrary(const QString& libraryName)
{
    if (!m_libraries.contains(libraryName)) {
        return false;
    }

    m_libraries.remove(libraryName);
    setModified(true);
    return true;
}

LibraryReference Project::getLibrary(const QString& name) const
{
    return m_libraries.value(name);
}

Project::Statistics Project::getStatistics() const
{
    Statistics stats;
    stats.totalPOUs = m_pous.size();
    stats.programs = getPOUsByType(POUType::Program).size();
    stats.functions = getPOUsByType(POUType::Function).size();
    stats.functionBlocks = getPOUsByType(POUType::FunctionBlock).size();
    stats.libraries = m_libraries.size();
    
    // 统计代码行数
    stats.codeLines = 0;
    for (const auto& pou : m_pous.values()) {
        stats.codeLines += pou->code().count('\n') + 1;
    }
    
    return stats;
}

void Project::setModified(bool modified)
{
    m_modified = modified;
    if (modified) {
        updateModifiedTime();
    }
}

void Project::updateModifiedTime()
{
    m_modifiedTime = QDateTime::currentDateTime();
}

bool Project::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        spdlog::error("Failed to open project file: {}", filePath.toStdString());
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        spdlog::error("Invalid project file format");
        return false;
    }

    QJsonObject root = doc.object();

    // 基本信息
    m_name = root["name"].toString();
    m_path = filePath;
    m_version = root["version"].toString("1.0.0");
    m_author = root["author"].toString();
    m_description = root["description"].toString();
    m_createdTime = QDateTime::fromString(root["createdTime"].toString(), Qt::ISODate);
    m_modifiedTime = QDateTime::fromString(root["modifiedTime"].toString(), Qt::ISODate);

    // 配置
    if (root.contains("config")) {
        QVariantMap configData = root["config"].toObject().toVariantMap();
        m_config.fromVariant(configData);
    }

    // 加载POUs
    m_pous.clear();
    if (root.contains("pous")) {
        QJsonArray pousArray = root["pous"].toArray();
        for (const QJsonValue& value : pousArray) {
            QJsonObject pouObj = value.toObject();
            
            // 创建POU对象
            POUType type = static_cast<POUType>(pouObj["type"].toInt());
            QString name = pouObj["name"].toString();
            
            std::shared_ptr<POU> pou;
            switch (type) {
                case POUType::Program:
                    pou = std::make_shared<Program>(name);
                    break;
                case POUType::Function:
                    pou = std::make_shared<Function>(name);
                    break;
                case POUType::FunctionBlock:
                    pou = std::make_shared<FunctionBlock>(name);
                    break;
            }
            
            if (pou) {
                pou->fromVariant(pouObj.toVariantMap());
                m_pous[name] = pou;
            }
        }
    }

    // 加载库
    m_libraries.clear();
    if (root.contains("libraries")) {
        QJsonArray libsArray = root["libraries"].toArray();
        for (const QJsonValue& value : libsArray) {
            QJsonObject libObj = value.toObject();
            LibraryReference lib;
            lib.fromVariant(libObj.toVariantMap());
            m_libraries[lib.name()] = lib;
        }
    }

    m_modified = false;
    spdlog::info("Project loaded: {}", m_name.toStdString());
    return true;
}

bool Project::saveToFile(const QString& filePath)
{
    QJsonObject root;

    // 基本信息
    root["name"] = m_name;
    root["version"] = m_version;
    root["author"] = m_author;
    root["description"] = m_description;
    root["createdTime"] = m_createdTime.toString(Qt::ISODate);
    root["modifiedTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // 配置
    QVariantMap configData = m_config.toVariant();
    root["config"] = QJsonObject::fromVariantMap(configData);

    // 保存POUs
    QJsonArray pousArray;
    for (const auto& pou : m_pous.values()) {
        QVariantMap pouData = pou->toVariant();
        pousArray.append(QJsonObject::fromVariantMap(pouData));
    }
    root["pous"] = pousArray;

    // 保存库
    QJsonArray libsArray;
    for (const auto& lib : m_libraries.values()) {
        QVariantMap libData = lib.toVariant();
        libsArray.append(QJsonObject::fromVariantMap(libData));
    }
    root["libraries"] = libsArray;

    // 写入文件
    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        spdlog::error("Failed to write project file: {}", filePath.toStdString());
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    m_path = filePath;
    m_modified = false;
    spdlog::info("Project saved: {}", m_name.toStdString());
    return true;
}

bool Project::exportToZip(const QString& zipPath)
{
    // TODO: 实现ZIP导出
    spdlog::info("Export to ZIP: {}", zipPath.toStdString());
    return false;
}

bool Project::importFromZip(const QString& zipPath)
{
    // TODO: 实现ZIP导入
    spdlog::info("Import from ZIP: {}", zipPath.toStdString());
    return false;
}

} // namespace models
} // namespace nascode
