#include "xbmclibrary.h"
#include "libraryitem.h"

XbmcLibrary::XbmcLibrary(XbmcModel *parent) :XbmcModel(parent)
{

}

QVariant XbmcLibrary::data(const QModelIndex &index, int role) const
{
    return m_list.at(index.row())->data(role);
}

XbmcModel* XbmcLibrary::exit()
{
    deleteLater();
    return m_parentModel;
}

QVariant XbmcLibrary::get(int row, const QString &roleName)
{
    return data(index(row, 0, QModelIndex()), roleNames().key(roleName.toAscii()));
}

XbmcModelItem *XbmcLibrary::getItem(int index)
{
    qDebug() << "returning: " << m_list.at(index);
    return m_list.at(index);
}
