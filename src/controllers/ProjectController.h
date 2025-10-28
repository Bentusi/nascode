#pragma once

#include <QObject>
#include "../models/ProjectModel.h"
#include "../views/MainWindow.h"

namespace nascode {
namespace controllers {

/**
 * @brief 工程控制器 - MVC中的Controller层
 * 协调Model和View,处理用户交互逻辑
 */
class ProjectController : public QObject
{
    Q_OBJECT

public:
    explicit ProjectController(models::ProjectModel* model, 
                              views::MainWindow* view,
                              QObject* parent = nullptr);
    ~ProjectController();

public slots:
    /**
     * @brief 创建新工程
     */
    bool createNewProject(const QString& name, const QString& path);

    /**
     * @brief 打开工程
     */
    bool openProject(const QString& filePath);

    /**
     * @brief 保存工程
     */
    bool saveProject();

    /**
     * @brief 关闭工程
     */
    void closeProject();

    /**
     * @brief 添加POU
     */
    bool addPOU(models::ProjectModel::ProjectItemType type, const QString& name);

    /**
     * @brief 删除POU
     */
    bool removePOU(const QString& name);

    /**
     * @brief 编译工程
     */
    bool buildProject();

signals:
    void projectCreated(const QString& projectPath);
    void projectOpened(const QString& projectPath);
    void projectSaved();
    void projectClosed();
    void buildStarted();
    void buildFinished(bool success);
    void errorOccurred(const QString& error);

private:
    models::ProjectModel* m_model;
    views::MainWindow* m_view;
};

} // namespace controllers
} // namespace nascode
