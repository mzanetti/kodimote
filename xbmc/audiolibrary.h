#ifndef AUDIOLIBRARY_H
#define AUDIOLIBRARY_H

#include "xbmcmodel.h"

#include <QStandardItem>

class AudioLibrary : public XbmcModel
{
    Q_OBJECT
public:
    explicit AudioLibrary();

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

};

#endif // AUDIOLIBRARY_H
