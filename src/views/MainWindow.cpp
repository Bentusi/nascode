#include "MainWindow.h"
#include "CodeEditor.h"
#include "OutputPanel.h"
#include "../controllers/ProjectController.h"
#include "../models/ProjectModel.h"
#include "../core/Application.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QIcon>
#include <QCloseEvent>
#include <QActionGroup>
#include <QDir>
#include <QFileInfo>
#include <spdlog/spdlog.h>

namespace nascode {
namespace views {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_projectController(nullptr)
    , m_projectModel(nullptr)
{
    setWindowTitle("NasCode - IEC-61131 ST Programming Environment");
    resize(1280, 800);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    // 创建中央部件
    m_centralSplitter = new QSplitter(Qt::Horizontal, this);
    
    // 工程树
    m_projectTree = new QTreeView(this);
    m_projectTree->setContextMenuPolicy(Qt::CustomContextMenu);
    m_centralSplitter->addWidget(m_projectTree);

    // 编辑器标签页
    m_editorTabs = new QTabWidget(this);
    m_editorTabs->setTabsClosable(true);
    m_editorTabs->setMovable(true);
    m_centralSplitter->addWidget(m_editorTabs);

    m_centralSplitter->setStretchFactor(0, 1);
    m_centralSplitter->setStretchFactor(1, 3);

    setCentralWidget(m_centralSplitter);

    setupConnections();
    retranslateUi();

    statusBar()->showMessage(tr("Ready"), 2000);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // TODO: 检查未保存的更改
    QMainWindow::closeEvent(event);
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::createActions()
{
    // 文件菜单动作
    m_newProjectAction = new QAction(tr("&New Project..."), this);
    m_newProjectAction->setShortcut(QKeySequence::New);
    m_newProjectAction->setStatusTip(tr("Create a new project"));

    m_openProjectAction = new QAction(tr("&Open Project..."), this);
    m_openProjectAction->setShortcut(QKeySequence::Open);
    m_openProjectAction->setStatusTip(tr("Open an existing project"));

    m_saveProjectAction = new QAction(tr("&Save"), this);
    m_saveProjectAction->setShortcut(QKeySequence::Save);
    m_saveProjectAction->setStatusTip(tr("Save the current project"));

    m_closeProjectAction = new QAction(tr("&Close Project"), this);
    m_closeProjectAction->setStatusTip(tr("Close the current project"));

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Exit the application"));

    // 编辑菜单动作
    m_undoAction = new QAction(tr("&Undo"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);

    m_redoAction = new QAction(tr("&Redo"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);

    m_cutAction = new QAction(tr("Cu&t"), this);
    m_cutAction->setShortcut(QKeySequence::Cut);

    m_copyAction = new QAction(tr("&Copy"), this);
    m_copyAction->setShortcut(QKeySequence::Copy);

    m_pasteAction = new QAction(tr("&Paste"), this);
    m_pasteAction->setShortcut(QKeySequence::Paste);

    // 构建菜单动作
    m_buildAction = new QAction(tr("&Build"), this);
    m_buildAction->setShortcut(Qt::Key_F7);
    m_buildAction->setStatusTip(tr("Build the project"));

    m_rebuildAction = new QAction(tr("&Rebuild"), this);
    m_cleanAction = new QAction(tr("&Clean"), this);

    // 调试菜单动作
    m_downloadAction = new QAction(tr("&Download to Device"), this);
    m_downloadAction->setStatusTip(tr("Download program to STVM"));

    m_startDebugAction = new QAction(tr("&Start Debugging"), this);
    m_startDebugAction->setShortcut(Qt::Key_F5);

    m_stopDebugAction = new QAction(tr("S&top Debugging"), this);
    m_stopDebugAction->setShortcut(Qt::SHIFT | Qt::Key_F5);

    m_pauseDebugAction = new QAction(tr("&Pause"), this);
    
    // 语言切换动作
    m_languageActionGroup = new QActionGroup(this);
    m_englishAction = new QAction(tr("English"), this);
    m_englishAction->setCheckable(true);
    m_englishAction->setData("English");
    m_languageActionGroup->addAction(m_englishAction);
    
    m_chineseAction = new QAction(tr("Chinese"), this);
    m_chineseAction->setCheckable(true);
    m_chineseAction->setData("Chinese");
    m_languageActionGroup->addAction(m_chineseAction);
    
    // 根据当前语言设置选中状态
    auto& i18n = nascode::core::I18nManager::getInstance();
    if (i18n.currentLanguage() == nascode::core::I18nManager::Chinese) {
        m_chineseAction->setChecked(true);
    } else {
        m_englishAction->setChecked(true);
    }
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newProjectAction);
    m_fileMenu->addAction(m_openProjectAction);
    m_fileMenu->addAction(m_saveProjectAction);
    m_fileMenu->addAction(m_closeProjectAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_cutAction);
    m_editMenu->addAction(m_copyAction);
    m_editMenu->addAction(m_pasteAction);

    m_buildMenu = menuBar()->addMenu(tr("&Build"));
    m_buildMenu->addAction(m_buildAction);
    m_buildMenu->addAction(m_rebuildAction);
    m_buildMenu->addAction(m_cleanAction);

    m_debugMenu = menuBar()->addMenu(tr("&Debug"));
    m_debugMenu->addAction(m_downloadAction);
    m_debugMenu->addSeparator();
    m_debugMenu->addAction(m_startDebugAction);
    m_debugMenu->addAction(m_pauseDebugAction);
    m_debugMenu->addAction(m_stopDebugAction);

    m_toolsMenu = menuBar()->addMenu(tr("&Tools"));
    
    // 语言子菜单
    m_languageMenu = m_toolsMenu->addMenu(tr("Language"));
    m_languageMenu->addAction(m_englishAction);
    m_languageMenu->addAction(m_chineseAction);
    
    m_helpMenu = menuBar()->addMenu(tr("&Help"));
}

void MainWindow::createToolBars()
{
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->addAction(m_newProjectAction);
    m_fileToolBar->addAction(m_openProjectAction);
    m_fileToolBar->addAction(m_saveProjectAction);

    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->addAction(m_undoAction);
    m_editToolBar->addAction(m_redoAction);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_cutAction);
    m_editToolBar->addAction(m_copyAction);
    m_editToolBar->addAction(m_pasteAction);

    m_buildToolBar = addToolBar(tr("Build"));
    m_buildToolBar->addAction(m_buildAction);

    m_debugToolBar = addToolBar(tr("Debug"));
    m_debugToolBar->addAction(m_downloadAction);
    m_debugToolBar->addAction(m_startDebugAction);
    m_debugToolBar->addAction(m_pauseDebugAction);
    m_debugToolBar->addAction(m_stopDebugAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    // 输出面板
    m_outputDock = new QDockWidget(tr("Output"), this);
    m_outputPanel = new OutputPanel(m_outputDock);
    m_outputDock->setWidget(m_outputPanel);
    addDockWidget(Qt::BottomDockWidgetArea, m_outputDock);

    // 变量监视窗口
    m_watchDock = new QDockWidget(tr("Watch"), this);
    QWidget* watchWidget = new QWidget();
    m_watchDock->setWidget(watchWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_watchDock);

    // 库浏览器
    m_libraryDock = new QDockWidget(tr("Library"), this);
    QWidget* libraryWidget = new QWidget();
    m_libraryDock->setWidget(libraryWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_libraryDock);
}

void MainWindow::setProjectController(controllers::ProjectController* controller)
{
    m_projectController = controller;
}

void MainWindow::setProjectModel(models::ProjectModel* model)
{
    m_projectModel = model;
    if (m_projectTree && model) {
        m_projectTree->setModel(model);
    }
}

void MainWindow::setupConnections()
{
    // 文件菜单
    connect(m_newProjectAction, &QAction::triggered, this, &MainWindow::onNewProject);
    connect(m_openProjectAction, &QAction::triggered, this, &MainWindow::onOpenProject);
    connect(m_saveProjectAction, &QAction::triggered, this, &MainWindow::onSaveProject);
    connect(m_closeProjectAction, &QAction::triggered, this, &MainWindow::onCloseProject);
    connect(m_exitAction, &QAction::triggered, this, &QMainWindow::close);
    connect(m_buildAction, &QAction::triggered, this, &MainWindow::onBuild);
    connect(m_downloadAction, &QAction::triggered, this, &MainWindow::onDownload);
    connect(m_startDebugAction, &QAction::triggered, this, &MainWindow::onStartDebug);

    connect(m_projectTree, &QTreeView::customContextMenuRequested, 
            this, &MainWindow::onProjectTreeContextMenu);
    connect(m_projectTree, &QTreeView::doubleClicked, 
            this, &MainWindow::onProjectItemDoubleClicked);
    
    // 语言切换
    connect(m_languageActionGroup, &QActionGroup::triggered, 
            this, &MainWindow::onLanguageChanged);
}

void MainWindow::retranslateUi()
{
    // 更新窗口标题
    setWindowTitle(tr("NasCode - IEC-61131 ST Programming Environment"));
    
    // 更新菜单
    m_fileMenu->setTitle(tr("&File"));
    m_editMenu->setTitle(tr("&Edit"));
    m_buildMenu->setTitle(tr("&Build"));
    m_debugMenu->setTitle(tr("&Debug"));
    m_toolsMenu->setTitle(tr("&Tools"));
    m_helpMenu->setTitle(tr("&Help"));
    
    // 更新语言菜单
    m_languageMenu->setTitle(tr("Language"));
    m_englishAction->setText(tr("English"));
    m_chineseAction->setText(tr("Chinese"));
    
    // 更新工具栏
    m_fileToolBar->setWindowTitle(tr("File"));
    m_editToolBar->setWindowTitle(tr("Edit"));
    m_buildToolBar->setWindowTitle(tr("Build"));
    m_debugToolBar->setWindowTitle(tr("Debug"));
    
    // 更新停靠窗口
    m_outputDock->setWindowTitle(tr("Output"));
    m_watchDock->setWindowTitle(tr("Watch"));
    m_libraryDock->setWindowTitle(tr("Library"));
    
    // 更新状态栏
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::onNewProject()
{
    spdlog::info("New project requested");
    
    if (!m_projectController) {
        QMessageBox::warning(this, tr("Error"), tr("Project controller not initialized"));
        return;
    }
    
    // 获取项目名称
    bool ok;
    QString projectName = QInputDialog::getText(this, tr("New Project"),
                                                tr("Project name:"), QLineEdit::Normal,
                                                "MyProject", &ok);
    if (!ok || projectName.isEmpty()) {
        return;
    }
    
    // 选择项目保存位置
    QString projectPath = QFileDialog::getSaveFileName(this,
        tr("Create New Project"),
        QDir::homePath() + "/" + projectName + ".nascode",
        tr("NasCode Project Files (*.nascode)"));
    
    if (projectPath.isEmpty()) {
        return;
    }
    
    // 确保有正确的扩展名
    if (!projectPath.endsWith(".nascode")) {
        projectPath += ".nascode";
    }
    
    // 创建项目
    if (m_projectController->createNewProject(projectName, projectPath)) {
        statusBar()->showMessage(tr("Project created: %1").arg(projectName), 3000);
        spdlog::info("Project created successfully: {}", projectName.toStdString());
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Failed to create project"));
    }
}

void MainWindow::onOpenProject()
{
    spdlog::info("Open project requested");
    
    if (!m_projectController) {
        QMessageBox::warning(this, tr("Error"), tr("Project controller not initialized"));
        return;
    }
    
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Project"),
        QDir::homePath(),
        tr("NasCode Project Files (*.nascode);;All Files (*)"));
    
    if (filePath.isEmpty()) {
        return;
    }
    
    if (m_projectController->openProject(filePath)) {
        statusBar()->showMessage(tr("Project opened: %1").arg(QFileInfo(filePath).baseName()), 3000);
        spdlog::info("Project opened successfully: {}", filePath.toStdString());
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open project: %1").arg(filePath));
    }
}

void MainWindow::onSaveProject()
{
    spdlog::info("Save project requested");
    
    if (!m_projectController) {
        QMessageBox::warning(this, tr("Error"), tr("Project controller not initialized"));
        return;
    }
    
    if (m_projectController->saveProject()) {
        statusBar()->showMessage(tr("Project saved"), 2000);
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Failed to save project"));
    }
}

void MainWindow::onCloseProject()
{
    spdlog::info("Close project requested");
    
    if (!m_projectController) {
        return;
    }
    
    // TODO: 检查未保存的更改
    m_projectController->closeProject();
    statusBar()->showMessage(tr("Project closed"), 2000);
}

void MainWindow::onUndo() {}
void MainWindow::onRedo() {}
void MainWindow::onCut() {}
void MainWindow::onCopy() {}
void MainWindow::onPaste() {}

void MainWindow::onBuild()
{
    spdlog::info("Build requested");
    m_outputPanel->appendMessage("Building project...");
    // TODO: 触发编译
}

void MainWindow::onRebuild() {}
void MainWindow::onClean() {}

void MainWindow::onDownload()
{
    spdlog::info("Download to device requested");
    m_outputPanel->appendMessage("Downloading to STVM...");
    // TODO: 下载程序
}

void MainWindow::onStartDebug()
{
    spdlog::info("Start debugging requested");
    // TODO: 启动调试
}

void MainWindow::onStopDebug() {}
void MainWindow::onPauseDebug() {}

void MainWindow::onProjectTreeContextMenu(const QPoint& pos)
{
    // TODO: 显示右键菜单
}

void MainWindow::onProjectItemDoubleClicked(const QModelIndex& index)
{
    // TODO: 打开编辑器
}

void MainWindow::onLanguageChanged()
{
    QAction* action = m_languageActionGroup->checkedAction();
    if (!action) return;
    
    QString langName = action->data().toString();
    auto& i18n = nascode::core::I18nManager::getInstance();
    auto lang = nascode::core::I18nManager::languageFromString(langName);
    
    if (lang != i18n.currentLanguage()) {
        i18n.switchLanguage(lang);
        
        // 提示重启应用以完全生效
        QMessageBox::information(this,
            tr("Language"),
            tr("Language changed. Please restart the application for complete effect."));
    }
}

} // namespace views
} // namespace nascode
