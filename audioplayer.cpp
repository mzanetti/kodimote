#include "audioplayer.h"

#include "audioplaylist.h"

namespace Xbmc
{

AudioPlayer::AudioPlayer(QObject *parent):
    Player(PlayerTypeAudio, parent)
{
    m_playList = new AudioPlaylist(this);
}

QString AudioPlayer::namespaceString() const
{
    return "AudioPlayer";
}

Playlist * AudioPlayer::playlist() const
{
    return m_playList;
}

}
