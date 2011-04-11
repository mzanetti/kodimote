#include "audioplaylist.h"

#include "xbmcconnection.h"

namespace Xbmc
{

AudioPlaylist::AudioPlaylist(QObject *parent):
    Playlist(parent)
{
}

QString AudioPlaylist::namespaceString() const
{
    return "AudioPlaylist";
}

}
