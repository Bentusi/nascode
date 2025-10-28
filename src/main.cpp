#include <QApplication>
#include <spdlog/spdlog.h>
#include "views/MainWindow.h"
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

    // 创建主窗口
    nascode::views::MainWindow mainWindow;
    mainWindow.show();

    int result = app.exec();
    
    spdlog::info("NasCode Exiting...");
    return result;
}
