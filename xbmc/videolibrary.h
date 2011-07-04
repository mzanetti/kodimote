#ifndef VIDEOLIBRARY_H
#define VIDEOLIBRARY_H

#include "xbmcmodel.h"

#include <QStandardItem>

class VideoLibrary : public XbmcModel
{
    Q_OBJECT
public:
    explicit VideoLibrary(XbmcModel *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    XbmcModel *enterItem(int index);
    void playItem(int index);

    QString title() const;

private:
    enum Request {
        RequestArtists,
        RequestAlbums
    };
    QMap<int, Request> m_requestMap;

    QStringList m_list;

};

#endif // VIDEOLIBRARY_H
