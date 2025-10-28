#include <QApplication>
#include <QMessageBox>
#include <spdlog/spdlog.h>
#include "views/MainWindow.h"
#include "models/ProjectModel.h"
#include "controllers/ProjectController.h"
#include "core/Application.h"

int main(int argc, char *argv[])
{
    // 初始化日志系统
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("NasCode Starting...");

    QApplication app(argc, argv);
    app.setApplicationName("NasCode");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("NasCode");

    // 初始化应用核心
    nascode::core::Application::getInstance().initialize();

    // 创建MVC组件
    nascode::views::MainWindow mainWindow;
    nascode::models::ProjectModel projectModel;
    nascode::controllers::ProjectController projectController(&projectModel, &mainWindow);

    // 连接MVC组件
    mainWindow.setProjectController(&projectController);
    mainWindow.setProjectModel(&projectModel);

    // 连接信号
    QObject::connect(&projectController, &nascode::controllers::ProjectController::projectCreated,
        [&mainWindow](const QString& path) {
            mainWindow.statusBar()->showMessage(QObject::tr("Project created: %1").arg(path), 3000);
        });

    QObject::connect(&projectController, &nascode::controllers::ProjectController::projectOpened,
        [&mainWindow](const QString& path) {
            mainWindow.statusBar()->showMessage(QObject::tr("Project opened: %1").arg(path), 3000);
        });

    QObject::connect(&projectController, &nascode::controllers::ProjectController::errorOccurred,
        [&mainWindow](const QString& error) {
            QMessageBox::critical(&mainWindow, QObject::tr("Error"), error);
        });

    mainWindow.show();

    int result = app.exec();
    
    spdlog::info("NasCode Exiting...");
    return result;
}
