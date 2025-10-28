#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QTabWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <memory>

namespace nascode {

namespace controllers {
class ProjectController;
}

namespace models {
class ProjectModel;
}

namespace views {

class CodeEditor;
class OutputPanel;

/**
 * @brief 主窗口 - View层核心
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    // 设置MVC组件
    void setProjectController(controllers::ProjectController* controller);
    void setProjectModel(models::ProjectModel* model);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    // 文件菜单
    void onNewProject();
    void onOpenProject();
    void onSaveProject();
    void onCloseProject();

    // 编辑菜单
    void onUndo();
    void onRedo();
    void onCut();
    void onCopy();
    void onPaste();

    // 构建菜单
    void onBuild();
    void onRebuild();
    void onClean();

    // 调试菜单
    void onDownload();
    void onStartDebug();
    void onStopDebug();
    void onPauseDebug();

    // 工具菜单
    void onLanguageChanged();

    // 工程树操作
    void onProjectTreeContextMenu(const QPoint& pos);
    void onProjectItemDoubleClicked(const QModelIndex& index);

protected:
    void changeEvent(QEvent* event) override;

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void setupConnections();
    void retranslateUi();

private:
    // 中央部件
    QSplitter* m_centralSplitter;
    QTreeView* m_projectTree;
    QTabWidget* m_editorTabs;

    // 输出面板
    OutputPanel* m_outputPanel;
    QDockWidget* m_outputDock;
    QDockWidget* m_watchDock;
    QDockWidget* m_libraryDock;

    // MVC组件
    controllers::ProjectController* m_projectController;
    models::ProjectModel* m_projectModel;

    // 菜单
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_buildMenu;
    QMenu* m_debugMenu;
    QMenu* m_toolsMenu;
    QMenu* m_helpMenu;

    // 工具栏
    QToolBar* m_fileToolBar;
    QToolBar* m_editToolBar;
    QToolBar* m_buildToolBar;
    QToolBar* m_debugToolBar;

    // 动作
    QAction* m_newProjectAction;
    QAction* m_openProjectAction;
    QAction* m_saveProjectAction;
    QAction* m_closeProjectAction;
    QAction* m_exitAction;

    QAction* m_undoAction;
    QAction* m_redoAction;
    QAction* m_cutAction;
    QAction* m_copyAction;
    QAction* m_pasteAction;

    QAction* m_buildAction;
    QAction* m_rebuildAction;
    QAction* m_cleanAction;

    QAction* m_downloadAction;
    QAction* m_startDebugAction;
    QAction* m_stopDebugAction;
    QAction* m_pauseDebugAction;

    // 语言菜单
    QMenu* m_languageMenu;
    QActionGroup* m_languageActionGroup;
    QAction* m_englishAction;
    QAction* m_chineseAction;
};

} // namespace views
} // namespace nascode
