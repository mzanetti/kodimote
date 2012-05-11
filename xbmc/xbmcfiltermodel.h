#ifndef XBMCFILTERMODEL_H
#define XBMCFILTERMODEL_H

#include "xbmcmodel.h"

#include <QSortFilterProxyModel>

class XbmcFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QObject* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool caseSensitive READ caseSensitive WRITE setCaseSensitive NOTIFY caseSensitivityChanged)

public:
    explicit XbmcFilterModel(QObject *parent = 0);
    
    QAbstractItemModel *model();
    void setModel(QObject* model);

    void setFilter(const QString &filter);
    QString filter();

    void setCaseSensitive(bool cs);
    bool caseSensitive();

    Q_INVOKABLE int mapToSourceIndex(int index);

signals:
    void modelChanged();
    void filterChanged();
    void caseSensitivityChanged();

private:
    QString m_filterString;
    bool m_cs;
};

#endif // XBMCFILTERMODEL_H
