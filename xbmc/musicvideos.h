#ifndef MUSICVIDEOS_H
#define MUSICVIDEOS_H

#include "xbmcmodel.h"

#include <QStandardItem>

class MusicVideos : public XbmcModel
{
    Q_OBJECT
public:
    explicit MusicVideos(XbmcModel *parent = 0);

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

#endif // MUSICVIDEOS_H
