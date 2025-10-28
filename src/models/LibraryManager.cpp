#include "LibraryManager.h"
#include "ProjectConfig.h"
#include <spdlog/spdlog.h>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

namespace nascode {
namespace models {

LibraryManager& LibraryManager::instance()
{
    static LibraryManager instance;
    return instance;
}

LibraryManager::LibraryManager()
{
    // 添加默认库路径
    QString appDir = QCoreApplication::applicationDirPath();
    addLibraryPath(appDir + "/libraries");
    
    // 用户库路径
    QString userLibDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/libraries";
    addLibraryPath(userLibDir);
    
    // 加载标准库
    loadStandardLibraries();
}

void LibraryManager::addLibraryPath(const QString& path)
{
    if (!m_libraryPaths.contains(path)) {
        m_libraryPaths.append(path);
        
        // 创建目录如果不存在
        QDir dir;
        if (!dir.exists(path)) {
            dir.mkpath(path);
        }
        
        spdlog::info("Added library path: {}", path.toStdString());
    }
}

void LibraryManager::removeLibraryPath(const QString& path)
{
    m_libraryPaths.removeAll(path);
}

void LibraryManager::scanLibraries()
{
    m_availableLibraries.clear();
    
    for (const QString& path : m_libraryPaths) {
        QDir dir(path);
        if (!dir.exists()) {
            continue;
        }
        
        // 扫描.lib文件
        QStringList filters;
        filters << "*.lib" << "*.xml";
        
        QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
        for (const QFileInfo& fileInfo : files) {
            loadLibraryMetadata(fileInfo.absoluteFilePath());
        }
    }
    
    emit librariesChanged();
    spdlog::info("Scanned {} libraries", m_availableLibraries.size());
}

void LibraryManager::loadLibraryMetadata(const QString& path)
{
    // TODO: 实现库元数据解析
    QFileInfo fileInfo(path);
    QString name = fileInfo.baseName();
    
    LibraryReference lib(name, path, "1.0.0");
    lib.setDescription("Library: " + name);
    
    m_availableLibraries[name] = lib;
}

QList<LibraryReference> LibraryManager::getAvailableLibraries() const
{
    return m_availableLibraries.values();
}

LibraryReference LibraryManager::getLibraryInfo(const QString& name) const
{
    return m_availableLibraries.value(name);
}

void LibraryManager::loadStandardLibraries()
{
    // 标准函数库
    LibraryReference stdLib("Standard", "", "1.0.0");
    stdLib.setDescription("IEC 61131-3 Standard Functions");
    stdLib.setEnabled(true);
    m_availableLibraries["Standard"] = stdLib;
    
    // 字符串处理库
    LibraryReference stringLib("String", "", "1.0.0");
    stringLib.setDescription("String manipulation functions");
    stringLib.setEnabled(true);
    m_availableLibraries["String"] = stringLib;
    
    // 数学库
    LibraryReference mathLib("Math", "", "1.0.0");
    mathLib.setDescription("Mathematical functions");
    mathLib.setEnabled(true);
    m_availableLibraries["Math"] = mathLib;
    
    // 通信库
    LibraryReference commLib("Communication", "", "1.0.0");
    commLib.setDescription("Communication protocols");
    commLib.setEnabled(true);
    m_availableLibraries["Communication"] = commLib;
    
    spdlog::info("Loaded {} standard libraries", m_availableLibraries.size());
}

QList<LibraryReference> LibraryManager::getStandardLibraries() const
{
    QList<LibraryReference> standardLibs;
    QStringList stdLibNames = {"Standard", "String", "Math", "Communication"};
    
    for (const QString& name : stdLibNames) {
        if (m_availableLibraries.contains(name)) {
            standardLibs.append(m_availableLibraries[name]);
        }
    }
    
    return standardLibs;
}

} // namespace models
} // namespace nascode
