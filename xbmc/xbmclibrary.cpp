#include "xbmclibrary.h"

XbmcLibrary::XbmcLibrary(XbmcModel *parent) :XbmcModel(parent)
{

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
