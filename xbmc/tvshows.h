#ifndef TVSHOWS_H
#define TVSHOWS_H

#include "xbmcmodel.h"

#include <QStandardItem>

class TvShows : public XbmcModel
{
    Q_OBJECT
public:
    explicit TvShows(XbmcModel *parent = 0);
    ~TvShows();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    XbmcModel *enterItem(int index);
    void playItem(int index);

    QString title() const;

private slots:
    void responseReceived(int id, const QVariantMap &rsp);

private:
    int m_request;
};

#endif // TVSHOWS_H
