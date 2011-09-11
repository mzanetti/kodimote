#ifndef ALBUMS_H
#define ALBUMS_H

#include "xbmcmodel.h"

#include <QStandardItem>

class Albums : public XbmcModel
{
    Q_OBJECT
public:
    explicit Albums(int artistId = -1, XbmcModel *parent = 0);
    ~Albums();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    XbmcModel* enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;

private slots:
    void responseReceived(int id, const QVariantMap &map);

private:
    int m_request;
    int m_artistId;

};

#endif // ALBUMS_H
