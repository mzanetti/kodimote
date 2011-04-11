#ifndef AUDIOPLAYLIST_H
#define AUDIOPLAYLIST_H

#include "playlist.h"

namespace Xbmc
{

class AudioPlaylist : public Playlist
{
public:
    AudioPlaylist(QObject *parent = 0);

protected:
    QString namespaceString() const;
};

}

#endif // AUDIOPLAYLIST_H
