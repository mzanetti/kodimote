#ifndef EPISODES_H
#define EPISODES_H

#include "xbmcmodel.h"

#include <QStandardItem>

class Episodes : public XbmcModel
{
    Q_OBJECT
public:
    // seasonstring is a workaround to pass the label of the season because there is no sane way to qery the label here
    explicit Episodes(int tvshowid = -1, int seasonid = -1, const QString &seasonString = QString(), XbmcModel *parent = 0);

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
    int m_seasonid;
    QString m_seasonString;
};

#endif // EPISODES_H
