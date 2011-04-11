#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "player.h"

namespace Xbmc
{
class AudioPlaylist;

class AudioPlayer : public Player
{
public:
    AudioPlayer(QObject *parent = 0);

    Q_INVOKABLE virtual Playlist *playlist() const;

protected:
    virtual QString namespaceString() const;

private:
    AudioPlaylist *m_playList;
};
}


#endif // AUDIOPLAYER_H
