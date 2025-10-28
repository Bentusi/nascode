#pragma once

#include <QAbstractItemModel>
#include <QFileSystemWatcher>
#include <memory>
#include "Project.h"

namespace nascode {
namespace models {

/**
 * @brief 工程模型类 - 实现Qt Model/View架构的Model层
 * 管理工程的树形结构数据
 */
class ProjectModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum ProjectItemType {
        ProjectRoot,
        ProgramFolder,
        FunctionFolder,
        FunctionBlockFolder,
        LibraryFolder,
        ProgramItem,
        FunctionItem,
        FunctionBlockItem,
        LibraryItem
    };

    explicit ProjectModel(QObject* parent = nullptr);
    ~ProjectModel() override;

    // QAbstractItemModel接口实现
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    /**
     * @brief 创建新工程
     */
    bool createProject(const QString& name, const QString& path);

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
     * @brief 获取当前工程
     */
    Project* currentProject() const { return m_currentProject.get(); }

    /**
     * @brief 添加POU(程序组织单元)
     */
    bool addPOU(ProjectItemType type, const QString& name);

    /**
     * @brief 删除POU
     */
    bool removePOU(const QModelIndex& index);

signals:
    void projectOpened(const QString& projectPath);
    void projectClosed();
    void projectModified();
    void pouAdded(const QString& pouName);
    void pouRemoved(const QString& pouName);

private:
    class TreeItem;
    void setupModelData(TreeItem* parent);
    TreeItem* getItem(const QModelIndex& index) const;

private:
    std::unique_ptr<Project> m_currentProject;
    std::unique_ptr<TreeItem> m_rootItem;
    QFileSystemWatcher* m_fileWatcher;
};

} // namespace models
} // namespace nascode
