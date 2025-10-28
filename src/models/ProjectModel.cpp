#include "ProjectModel.h"
#include <spdlog/spdlog.h>

namespace nascode {
namespace models {

// TreeItem内部类实现
class ProjectModel::TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant>& data, TreeItem* parent = nullptr)
        : m_itemData(data), m_parentItem(parent)
    {}

    ~TreeItem() { qDeleteAll(m_childItems); }

    TreeItem* child(int row) { return m_childItems.value(row); }
    int childCount() const { return m_childItems.count(); }
    int columnCount() const { return m_itemData.count(); }
    QVariant data(int column) const { return m_itemData.value(column); }
    int row() const
    {
        if (m_parentItem)
            return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
        return 0;
    }
    TreeItem* parentItem() { return m_parentItem; }
    void appendChild(TreeItem* child) { m_childItems.append(child); }

private:
    QVector<TreeItem*> m_childItems;
    QVector<QVariant> m_itemData;
    TreeItem* m_parentItem;
};

ProjectModel::ProjectModel(QObject* parent)
    : QAbstractItemModel(parent)
    , m_fileWatcher(new QFileSystemWatcher(this))
{
    QVector<QVariant> rootData;
    rootData << "Name" << "Type";
    m_rootItem = std::make_unique<TreeItem>(rootData);
}

ProjectModel::~ProjectModel()
{
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem* parentItem = getItem(parent);
    TreeItem* childItem = parentItem->child(row);
    
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ProjectModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    TreeItem* childItem = getItem(child);
    TreeItem* parentItem = childItem->parentItem();

    if (parentItem == m_rootItem.get())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem = getItem(parent);
    return parentItem->childCount();
}

int ProjectModel::columnCount(const QModelIndex& parent) const
{
    return m_rootItem->columnCount();
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem* item = getItem(index);

    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    }

    return QVariant();
}

QVariant ProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);

    return QVariant();
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

ProjectModel::TreeItem* ProjectModel::getItem(const QModelIndex& index) const
{
    if (index.isValid()) {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem.get();
}

bool ProjectModel::createProject(const QString& name, const QString& path)
{
    m_currentProject = std::make_unique<Project>();
    m_currentProject->setName(name);
    m_currentProject->setPath(path);
    
    setupModelData(m_rootItem.get());
    
    emit projectOpened(path);
    return true;
}

bool ProjectModel::openProject(const QString& filePath)
{
    auto project = std::make_unique<Project>();
    if (!project->loadFromFile(filePath)) {
        return false;
    }

    m_currentProject = std::move(project);
    setupModelData(m_rootItem.get());
    
    emit projectOpened(filePath);
    return true;
}

bool ProjectModel::saveProject()
{
    if (!m_currentProject) {
        return false;
    }

    return m_currentProject->saveToFile(m_currentProject->path());
}

void ProjectModel::closeProject()
{
    if (m_currentProject) {
        beginResetModel();
        m_currentProject.reset();
        m_rootItem->~TreeItem();
        new (m_rootItem.get()) TreeItem(QVector<QVariant>{"Name", "Type"});
        endResetModel();
        emit projectClosed();
    }
}

void ProjectModel::setupModelData(TreeItem* parent)
{
    // TODO: 根据当前工程数据构建树形结构
}

bool ProjectModel::addPOU(ProjectItemType type, const QString& name)
{
    if (!m_currentProject) {
        return false;
    }

    std::shared_ptr<POU> pou;
    switch (type) {
        case ProgramItem:
            pou = std::make_shared<Program>(name);
            break;
        case FunctionItem:
            pou = std::make_shared<Function>(name);
            break;
        case FunctionBlockItem:
            pou = std::make_shared<FunctionBlock>(name);
            break;
        default:
            return false;
    }

    if (m_currentProject->addPOU(pou)) {
        emit pouAdded(name);
        return true;
    }

    return false;
}

bool ProjectModel::removePOU(const QModelIndex& index)
{
    // TODO: 实现POU删除
    return false;
}

} // namespace models
} // namespace nascode
