#pragma once

#include <QString>
#include <QStringList>
#include <QMap>
#include <memory>
#include "POU.h"

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

    // POU管理
    bool addPOU(std::shared_ptr<POU> pou);
    bool removePOU(const QString& name);
    std::shared_ptr<POU> getPOU(const QString& name) const;
    QList<std::shared_ptr<POU>> getAllPOUs() const;

    // 库管理
    bool addLibrary(const QString& libraryPath);
    bool removeLibrary(const QString& libraryName);
    QStringList getLibraries() const { return m_libraries; }

    // 序列化
    bool loadFromFile(const QString& filePath);
    bool saveToFile(const QString& filePath);

    bool isModified() const { return m_modified; }
    void setModified(bool modified) { m_modified = modified; }

private:
    QString m_name;
    QString m_path;
    QString m_version;
    QMap<QString, std::shared_ptr<POU>> m_pous;
    QStringList m_libraries;
    bool m_modified;
};

} // namespace models
} // namespace nascode
