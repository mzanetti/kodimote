#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "playlistitem.h"
#include "songitem.h"

#include <QObject>
#include <QVariantMap>
#include <QAbstractItemModel>

namespace Xbmc
{

class Playlist : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count)
    Q_PROPERTY(int currentTrackNumber READ currentTrackNumber NOTIFY currentChanged)
    Q_PROPERTY(QString currentLabel READ currentLabel NOTIFY currentChanged)
    Q_PROPERTY(QString currentTitle READ currentTitle NOTIFY currentChanged)
    Q_PROPERTY(QString currentArtist READ currentArtist NOTIFY currentChanged)
    Q_PROPERTY(QString currentAlbum READ currentAlbum NOTIFY currentChanged)
    Q_PROPERTY(QString currentFanart READ currentThumbnail NOTIFY currentChanged)
    Q_PROPERTY(QString currentThumbnail READ currentThumbnail NOTIFY currentChanged)

public:
    explicit Playlist(QObject *parent = 0);


    void clear();
    void addItems(const QList<SongItem> &itemList);

    int count() const;

    int currentTrackNumber() const;
    QString currentLabel() const;
    QString currentTitle() const;
    QString currentArtist() const;
    QString currentAlbum() const;
    QString currentFanart() const;
    QString currentThumbnail() const;


    SongItem at(int index);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

signals:
    void currentChanged();

public slots:
    void refresh();
    void playItem(int index);
    void skipNext();
    void skipPrevious();

private slots:
    void responseReveiced(int id, const QVariant &response);
    void receivedAnnouncement(const QVariantMap &map);

protected:
    virtual QString namespaceString() const = 0;

    void queryItemData(int index);

private:
    enum Request {
        RequestGetItems,
        RequestCurrentData
    };

    QMap<int, Request> m_requestMap;
    QList<SongItem> m_itemList;

    int m_currentSong;

};

}

#endif // PLAYLIST_H
