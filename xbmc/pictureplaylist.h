#ifndef PICTUREPLAYLIST_H
#define PICTUREPLAYLIST_H

#include "playlist.h"

class PicturePlaylist : public Playlist
{
public:
    PicturePlaylist();

    int rowCount(const QModelIndex &parent) const { return 0; };
    QVariant data(const QModelIndex &index, int role) const { return "bla"; };
    QString title() const {return "Pictures"; } ;
    PlaylistItem *at(int index) const { return 0; }
    int playlistId() const { return 2; };
    void refresh() {};
    void responseReveiced(int id, const QVariantMap &response) {};
    void queryItemData(int index) {};

};

#endif // PICTUREPLAYLIST_H
