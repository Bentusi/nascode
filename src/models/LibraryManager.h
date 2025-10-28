#pragma once

#include "ProjectConfig.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <memory>

namespace nascode {
namespace models {

class LibraryReference;

/**
 * @brief 库管理器 - 管理所有可用的库
 */
class LibraryManager : public QObject
{
    Q_OBJECT

public:
    static LibraryManager& instance();

    // 库路径管理
    void addLibraryPath(const QString& path);
    void removeLibraryPath(const QString& path);
    QStringList getLibraryPaths() const { return m_libraryPaths; }

    // 库发现和加载
    void scanLibraries();
    QList<LibraryReference> getAvailableLibraries() const;
    LibraryReference getLibraryInfo(const QString& name) const;

    // 标准库
    QList<LibraryReference> getStandardLibraries() const;
    void loadStandardLibraries();

signals:
    void librariesChanged();
    void libraryAdded(const QString& name);
    void libraryRemoved(const QString& name);

private:
    LibraryManager();
    ~LibraryManager() = default;
    LibraryManager(const LibraryManager&) = delete;
    LibraryManager& operator=(const LibraryManager&) = delete;

    void loadLibraryMetadata(const QString& path);

private:
    QStringList m_libraryPaths;
    QMap<QString, LibraryReference> m_availableLibraries;
};

} // namespace models
} // namespace nascode
