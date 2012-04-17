#include "xbmclibrary.h"
#include "libraryitem.h"

#include <QTimer>

XbmcLibrary::XbmcLibrary(XbmcModel *parent) :XbmcModel(parent)
{
    // Refresh the model automatically on the next event loop run.
    // This is to give QML time to create the object and set properties before the refresh
    QTimer::singleShot(0, this, SLOT(refresh()));
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
