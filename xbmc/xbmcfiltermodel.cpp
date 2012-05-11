#include "xbmcfiltermodel.h"

XbmcFilterModel::XbmcFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

QAbstractItemModel *XbmcFilterModel::model()
{
    return sourceModel();
}

void XbmcFilterModel::setModel(QObject *model)
{
    setSourceModel(static_cast<QAbstractItemModel*>(model));
    emit modelChanged();
}

void XbmcFilterModel::setFilter(const QString &filter)
{
    m_filterString = filter;
    setFilterFixedString(m_filterString);
    emit filterChanged();
}

QString XbmcFilterModel::filter()
{
    return m_filterString;
}

void XbmcFilterModel::setCaseSensitive(bool cs)
{
    setFilterCaseSensitivity(m_cs ? Qt::CaseSensitive : Qt::CaseInsensitive);
    emit caseSensitivityChanged();
}

bool XbmcFilterModel::caseSensitive()
{
    return filterCaseSensitivity() == Qt::CaseSensitive ? true : false;
}

int XbmcFilterModel::mapToSourceIndex(int i)
{
    return mapToSource(index(i, 0, QModelIndex())).row();
}

