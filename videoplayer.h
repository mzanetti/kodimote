#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "player.h"

namespace Xbmc
{

class VideoPlayer : public Player
{
    Q_OBJECT
public:
    VideoPlayer(QObject *parent = 0);

    virtual Playlist *playlist() const;

protected:
    virtual QString namespaceString() const;

private:

};
}

#endif // VIDEOPLAYER_H
