#include "playlists.h"

#include "kodiconnection.h"
#include "libraryitem.h"
#include "playlistcontent.h"

Playlists::Playlists(const QString &title, const QString &mediaType, const QString &dir, KodiModel *parent) :
    Files(mediaType, dir, parent),
    m_title(title)
{
}

QString Playlists::title() const
{
    return m_title;
}

KodiModel *Playlists::enterItem(int index)
{
    LibraryItem *item = static_cast<LibraryItem*>(m_list.at(index));
    if(item->fileType() == "directory") {
        return new PlaylistContent(item->title(), m_mediaType, item->fileName(), this);
    }
    qDebug() << "cannot enter item of type file";
    return 0;
}
