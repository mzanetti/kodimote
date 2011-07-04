#include "xbmcmodel.h"

#include <QDebug>

XbmcModel::XbmcModel(XbmcModel *parent) :
    QAbstractItemModel(parent),
    m_parentModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "title");
    roleNames.insert(Qt::UserRole+1, "filetype");
    roleNames.insert(Qt::UserRole+2, "subtitle");
    roleNames.insert(Qt::UserRole+3, "duration");
    setRoleNames(roleNames);
}

XbmcModel::~XbmcModel()
{
    while(!m_list.isEmpty()) {
        delete m_list.takeFirst();
    }
}

XbmcModel *XbmcModel::parentModel() const
{
    return m_parentModel;
}

XbmcModel* XbmcModel::exit()
{
    deleteLater();
    return m_parentModel;
}

int XbmcModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QModelIndex XbmcModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex XbmcModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}
