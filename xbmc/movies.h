#ifndef MOVIES_H
#define MOVIES_H

#include "xbmcmodel.h"

#include <QStandardItem>

class Movies : public XbmcModel
{
    Q_OBJECT
public:
    explicit Movies(XbmcModel *parent = 0);
    ~Movies();

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
};

#endif // MOVIES_H
