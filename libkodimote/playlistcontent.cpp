#include "playlistcontent.h"

#include "player.h"
#include "playlist.h"
#include "playlistitem.h"
#include "libraryitem.h"

PlaylistContent::PlaylistContent(const QString &title, const QString &mediaType, const QString &dir, KodiModel *parent) :
    Files(mediaType, dir, parent),
    m_title(title)
{
    //disable sorting, as otherwise the order in Kodi and our view would be different
    //meaning indexes used for "playItem" are wrong etc
    m_sort = false;
}

QString PlaylistContent::title() const
{
    return m_title;
}

void PlaylistContent::playItem(int index, bool resume)
{
    Q_UNUSED(resume)
    PlaylistItem item;
    item.setDirectory(m_dir);

    m_player->playlist()->clear();
    m_player->playlist()->addItems(item);
    m_player->playItem(index);
}

void PlaylistContent::addToPlaylist(int index)
{
    LibraryItem *item = static_cast<LibraryItem*>(m_list.at(index));
    PlaylistItem pItem;
    if(item->fileType() == "file") {
        pItem.setFile(item->fileName());
    } else {
        pItem.setDirectory(item->fileName());
    }
    m_player->playlist()->addItems(pItem);
}
