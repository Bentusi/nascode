#pragma once

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDateTime>
#include <memory>
#include "POU.h"
#include "ProjectConfig.h"

namespace nascode {
namespace models {

/**
 * @brief 工程数据类
 */
class Project
{
public:
    Project();
    ~Project();

    // 基本信息
    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    QString path() const { return m_path; }
    void setPath(const QString& path) { m_path = path; }

    QString version() const { return m_version; }
    void setVersion(const QString& version) { m_version = version; }

    QString author() const { return m_author; }
    void setAuthor(const QString& author) { m_author = author; }

    QString description() const { return m_description; }
    void setDescription(const QString& desc) { m_description = desc; }

    QDateTime createdTime() const { return m_createdTime; }
    QDateTime modifiedTime() const { return m_modifiedTime; }

    // 配置管理
    ProjectConfig& config() { return m_config; }
    const ProjectConfig& config() const { return m_config; }

    // POU管理
    bool addPOU(std::shared_ptr<POU> pou);
    bool removePOU(const QString& name);
    std::shared_ptr<POU> getPOU(const QString& name) const;
    QList<std::shared_ptr<POU>> getAllPOUs() const;
    QList<std::shared_ptr<POU>> getPOUsByType(POUType type) const;
    int getPOUCount() const { return m_pous.size(); }

    // 库管理
    bool addLibrary(const LibraryReference& library);
    bool removeLibrary(const QString& libraryName);
    LibraryReference getLibrary(const QString& name) const;
    QList<LibraryReference> getAllLibraries() const { return m_libraries.values(); }
    int getLibraryCount() const { return m_libraries.size(); }

    // 项目统计
    struct Statistics {
        int totalPOUs;
        int programs;
        int functions;
        int functionBlocks;
        int libraries;
        int codeLines;
    };
    Statistics getStatistics() const;

    // 序列化
    bool loadFromFile(const QString& filePath);
    bool saveToFile(const QString& filePath);
    
    // 导入导出
    bool exportToZip(const QString& zipPath);
    bool importFromZip(const QString& zipPath);

    bool isModified() const { return m_modified; }
    void setModified(bool modified);

private:
    void updateModifiedTime();

private:
    QString m_name;
    QString m_path;
    QString m_version;
    QString m_author;
    QString m_description;
    QDateTime m_createdTime;
    QDateTime m_modifiedTime;
    
    ProjectConfig m_config;
    QMap<QString, std::shared_ptr<POU>> m_pous;
    QMap<QString, LibraryReference> m_libraries;
    bool m_modified;
};

} // namespace models
} // namespace nascode
