#ifndef SEASONS_H
#define SEASONS_H

#include "xbmcmodel.h"

#include <QStandardItem>

class Seasons : public XbmcModel
{
    Q_OBJECT
public:
    explicit Seasons(int tvhsowId = -1, XbmcModel *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    XbmcModel *enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;

private slots:
    void responseReceived(int id, const QVariantMap &rsp);

private:
    int m_request;
    int m_tvshowid;
};

#endif // SEASONS_H
