#ifndef TITLES_H
#define TITLES_H

#include "xbmcmodel.h"

#include <QStandardItem>

class Songs : public XbmcModel
{
    Q_OBJECT
public:
    explicit Songs(int artistid = -1, int albumid = -1, XbmcModel *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    XbmcModel* enterItem(int index);
    void playItem(int index);

    QString title() const;

private slots:
    void responseReceived(int id, const QVariantMap &rsp);

private:
    int m_requestId;
    int m_artistId;
    int m_albumId;
};

#endif // TITLES_H
