#include "xbmcmodel.h"

#include <QDebug>

XbmcModel::XbmcModel(XbmcModel *parent) :
    QAbstractItemModel(parent),
    m_parentModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "title");
    roleNames.insert(RoleFileType, "filetype");
    roleNames.insert(RoleSubtitle, "subtitle");
    roleNames.insert(RoleDuration, "duration");
    roleNames.insert(RolePlayable, "playable");
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

int XbmcModel::findItem(const QString &string, bool caseSensitive)
{
    foreach(QStandardItem* item, m_list) {
        if(caseSensitive) {
            if(item->data(Qt::DisplayRole).toString().startsWith(string)) {
                return m_list.indexOf(item);
            }
        } else {
            if(item->data(Qt::DisplayRole).toString().toLower().startsWith(string.toLower())) {
                return m_list.indexOf(item);
            }
        }
    }
    return -1;
}
