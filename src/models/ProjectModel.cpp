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
    void removeChild(int row)
    {
        if (row >= 0 && row < m_childItems.size()) {
            m_childItems.removeAt(row);
        }
    }

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
    beginResetModel();
    
    m_currentProject = std::make_unique<Project>();
    m_currentProject->setName(name);
    m_currentProject->setPath(path);
    
    // 构建树形结构
    setupModelData(m_rootItem.get());
    
    // 保存项目文件
    if (!m_currentProject->saveToFile(path)) {
        m_currentProject.reset();
        endResetModel();
        return false;
    }
    
    endResetModel();
    emit projectOpened(path);
    return true;
}

bool ProjectModel::openProject(const QString& filePath)
{
    beginResetModel();
    
    auto project = std::make_unique<Project>();
    if (!project->loadFromFile(filePath)) {
        endResetModel();
        return false;
    }

    m_currentProject = std::move(project);
    setupModelData(m_rootItem.get());
    
    endResetModel();
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
    if (!m_currentProject) {
        return;
    }
    
    // 清空现有的子节点
    while (parent->childCount() > 0) {
        delete parent->child(0);
        parent->removeChild(0);
    }
    
    // 项目根节点
    TreeItem* projectRoot = new TreeItem(
        QVector<QVariant>() << m_currentProject->name() << "Project",
        parent
    );
    parent->appendChild(projectRoot);
    
    // Programs文件夹
    TreeItem* programsFolder = new TreeItem(
        QVector<QVariant>() << "Programs" << "Folder",
        projectRoot
    );
    projectRoot->appendChild(programsFolder);
    
    // 添加所有Program
    auto programs = m_currentProject->getPOUsByType(POUType::Program);
    for (const auto& pou : programs) {
        TreeItem* item = new TreeItem(
            QVector<QVariant>() << pou->name() << "Program",
            programsFolder
        );
        programsFolder->appendChild(item);
    }
    
    // Functions文件夹
    TreeItem* functionsFolder = new TreeItem(
        QVector<QVariant>() << "Functions" << "Folder",
        projectRoot
    );
    projectRoot->appendChild(functionsFolder);
    
    // 添加所有Function
    auto functions = m_currentProject->getPOUsByType(POUType::Function);
    for (const auto& pou : functions) {
        TreeItem* item = new TreeItem(
            QVector<QVariant>() << pou->name() << "Function",
            functionsFolder
        );
        functionsFolder->appendChild(item);
    }
    
    // Function Blocks文件夹
    TreeItem* fbsFolder = new TreeItem(
        QVector<QVariant>() << "Function Blocks" << "Folder",
        projectRoot
    );
    projectRoot->appendChild(fbsFolder);
    
    // 添加所有Function Block
    auto fbs = m_currentProject->getPOUsByType(POUType::FunctionBlock);
    for (const auto& pou : fbs) {
        TreeItem* item = new TreeItem(
            QVector<QVariant>() << pou->name() << "Function Block",
            fbsFolder
        );
        fbsFolder->appendChild(item);
    }
    
    // Libraries文件夹
    TreeItem* librariesFolder = new TreeItem(
        QVector<QVariant>() << "Libraries" << "Folder",
        projectRoot
    );
    projectRoot->appendChild(librariesFolder);
    
    // 添加所有库
    auto libraries = m_currentProject->getAllLibraries();
    for (const auto& lib : libraries) {
        TreeItem* item = new TreeItem(
            QVector<QVariant>() << lib.name() << QString("Library %1").arg(lib.version()),
            librariesFolder
        );
        librariesFolder->appendChild(item);
    }
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
        // 刷新树形结构
        beginResetModel();
        setupModelData(m_rootItem.get());
        endResetModel();
        
        emit pouAdded(name);
        return true;
    }

    return false;
}

bool ProjectModel::removePOU(const QModelIndex& index)
{
    if (!m_currentProject || !index.isValid()) {
        return false;
    }
    
    TreeItem* item = getItem(index);
    QString pouName = item->data(0).toString();
    
    if (m_currentProject->removePOU(pouName)) {
        beginResetModel();
        setupModelData(m_rootItem.get());
        endResetModel();
        
        emit pouRemoved(pouName);
        return true;
    }
    
    return false;
}

std::shared_ptr<POU> ProjectModel::getPOU(const QModelIndex& index) const
{
    if (!m_currentProject || !index.isValid()) {
        return nullptr;
    }
    
    TreeItem* item = getItem(index);
    QString pouName = item->data(0).toString();
    
    return m_currentProject->getPOU(pouName);
}

} // namespace models
} // namespace nascode
