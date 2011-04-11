#include "videoplayer.h"

namespace Xbmc
{

VideoPlayer::VideoPlayer(QObject *parent):
    Player(PlayerTypeVideo, parent)
{
}

QString VideoPlayer::namespaceString() const
{
    return "VideoPlayer";
}

Playlist *VideoPlayer::playlist() const
{
    //return m_playlist;
}

}
