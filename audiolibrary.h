#ifndef AUDIOLIBRARY_H
#define AUDIOLIBRARY_H

#include "libraryitem.h"
#include "artistitem.h"
#include "albumitem.h"
#include "songitem.h"

#include <QAbstractItemModel>

namespace Xbmc
{

class Player;

class AudioLibrary : public QAbstractItemModel

{
    Q_OBJECT

    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    Q_PROPERTY(int artistFilter READ artistFilter())
    Q_PROPERTY(int albumFilter READ albumFilter())

public:
    explicit AudioLibrary(Player *player, QObject *parent = 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QString state();
    int artistFilter();
    int albumFilter();

signals:
    void stateChanged();

public slots:
    void enterItem(int index);
    void showLibrary();
    void showArtists();
    void showAlbums(int artistId = -1);
    void showSongs(int artistId = -1, int albumId = -1);

private:
    enum Request {
        RequestArtists,
        RequestAlbums,
        RequestSongs
    };
    QMap<int, Request> m_requestMap;

    Player *m_player;

    QString m_state;
    int m_artistFilter;
    int m_albumFilter;

    QList<LibraryItem> m_list;
    QList<ArtistItem> m_artistList;
    QList<AlbumItem> m_albumList;
    QList<SongItem> m_songList;

private slots:
    void responseReceived(int,QVariant);


};

}
#endif // AUDIOLIBRARY_H
