#ifndef ARTISTS_H
#define ARTISTS_H

#include "xbmcmodel.h"

#include <QStandardItem>

class Artists : public XbmcModel
{
    Q_OBJECT
public:
    explicit Artists(XbmcModel *parent = 0);
    ~Artists();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    XbmcModel *enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;

private slots:
    void responseReceived(int id, const QVariantMap &rsp);

signals:

public slots:

private:
    int m_request;

};

#endif // ARTISTS_H
