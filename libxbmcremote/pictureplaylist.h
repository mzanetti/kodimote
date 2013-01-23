#ifndef PICTUREPLAYLIST_H
#define PICTUREPLAYLIST_H

#include "playlist.h"

class PicturePlaylist : public Playlist
{
public:
    PicturePlaylist();

    int rowCount(const QModelIndex &) const { return 0; }
    QVariant data(const QModelIndex &, int) const { return "bla"; }
    QString title() const {return "Pictures"; }
    PlaylistItem *at(int) const { return 0; }
    int playlistId() const { return 2; }
    void refresh() {}
    void queryItemData(int) {}

private slots:
    void receivedAnnouncement(const QVariantMap &map);

};

#endif // PICTUREPLAYLIST_H
