#include "ProjectController.h"
#include "../models/LibraryManager.h"
#include <spdlog/spdlog.h>

namespace nascode {
namespace controllers {

ProjectController::ProjectController(models::ProjectModel* model, 
                                    views::MainWindow* view,
                                    QObject* parent)
    : QObject(parent)
    , m_model(model)
    , m_view(view)
{
    // 连接Model的信号
    connect(m_model, &models::ProjectModel::projectOpened,
            this, &ProjectController::projectOpened);
    connect(m_model, &models::ProjectModel::projectClosed,
            this, &ProjectController::projectClosed);
}

ProjectController::~ProjectController()
{
}

bool ProjectController::createNewProject(const QString& name, const QString& path)
{
    spdlog::info("Creating new project: {} at {}", name.toStdString(), path.toStdString());
    
    if (m_model->createProject(name, path)) {
        emit projectCreated(path);
        return true;
    }

    emit errorOccurred("Failed to create project");
    return false;
}

bool ProjectController::openProject(const QString& filePath)
{
    spdlog::info("Opening project: {}", filePath.toStdString());
    
    if (m_model->openProject(filePath)) {
        return true;
    }

    emit errorOccurred("Failed to open project");
    return false;
}

bool ProjectController::saveProject()
{
    spdlog::info("Saving project");
    
    if (m_model->saveProject()) {
        emit projectSaved();
        return true;
    }

    emit errorOccurred("Failed to save project");
    return false;
}

void ProjectController::closeProject()
{
    spdlog::info("Closing project");
    m_model->closeProject();
}

bool ProjectController::addPOU(models::ProjectModel::ProjectItemType type, const QString& name)
{
    spdlog::info("Adding POU: {}", name.toStdString());
    
    if (m_model->addPOU(type, name)) {
        return true;
    }

    emit errorOccurred("Failed to add POU");
    return false;
}

bool ProjectController::removePOU(const QString& name)
{
    spdlog::info("Removing POU: {}", name.toStdString());
    
    // TODO: 实现POU删除逻辑
    return false;
}

bool ProjectController::buildProject()
{
    spdlog::info("Building project");
    emit buildStarted();

    // TODO: 实现编译逻辑
    // 1. 调用编译器
    // 2. 收集编译输出
    // 3. 显示错误/警告

    emit buildFinished(true);
    return true;
}

bool ProjectController::downloadToDevice()
{
    spdlog::info("Downloading to device");
    
    // TODO: 实现下载逻辑
    return false;
}

bool ProjectController::addLibrary(const QString& libraryName)
{
    spdlog::info("Adding library: {}", libraryName.toStdString());
    
    auto& libManager = models::LibraryManager::instance();
    models::LibraryReference lib = libManager.getLibraryInfo(libraryName);
    
    if (lib.name().isEmpty()) {
        emit errorOccurred("Library not found: " + libraryName);
        return false;
    }
    
    auto project = m_model->currentProject();
    if (!project) {
        emit errorOccurred("No project opened");
        return false;
    }
    
    if (project->addLibrary(lib)) {
        spdlog::info("Library added: {}", libraryName.toStdString());
        return true;
    }
    
    emit errorOccurred("Failed to add library");
    return false;
}

bool ProjectController::removeLibrary(const QString& libraryName)
{
    spdlog::info("Removing library: {}", libraryName.toStdString());
    
    auto project = m_model->currentProject();
    if (!project) {
        emit errorOccurred("No project opened");
        return false;
    }
    
    if (project->removeLibrary(libraryName)) {
        spdlog::info("Library removed: {}", libraryName.toStdString());
        return true;
    }
    
    emit errorOccurred("Failed to remove library");
    return false;
}

QList<models::LibraryReference> ProjectController::getAvailableLibraries() const
{
    auto& libManager = models::LibraryManager::instance();
    return libManager.getAvailableLibraries();
}

models::Project::Statistics ProjectController::getProjectStatistics() const
{
    auto project = m_model->currentProject();
    if (project) {
        return project->getStatistics();
    }
    return models::Project::Statistics();
}

} // namespace controllers
} // namespace nascode
