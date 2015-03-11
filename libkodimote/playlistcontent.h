#ifndef PLAYLISTCONTENT_H
#define PLAYLISTCONTENT_H

#include "files.h"

class PlaylistContent : public Files
{
    Q_OBJECT
public:
    explicit PlaylistContent(const QString &title, const QString &mediaType, const QString &dir, KodiModel *parent);

    QString title() const;
    void playItem(int index, bool resume = false);
    void addToPlaylist(int index);

private:
    QString m_title;
};

#endif // PLAYLISTCONTENT_H
