#include "mprisplayer.h"

#include "kodi.h"
#include "player.h"
#include "playlist.h"

MprisPlayer::MprisPlayer(QObject *parent) :
    QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

bool MprisPlayer::canControl() const
{
    return true;
}

bool MprisPlayer::canGoNext() const
{
    Player *player = activePlayer();
    if (!player) {
        return false;
    }

    return player->playlist()->currentTrackNumber() < player->playlist()->count();
}

bool MprisPlayer::canGoPrevious() const
{
    Player *player = activePlayer();
    if (!player) {
        return false;
    }

    return player->playlist()->currentTrackNumber() > 1;
}

bool MprisPlayer::canPause() const
{
    return true;
}

bool MprisPlayer::canPlay() const
{
    Player *player = activePlayer();
    if (!player) {
        return false;
    }

    return !!player->currentItem();
}

bool MprisPlayer::canSeek() const
{
    return false;
}

QVariantMap MprisPlayer::metadata() const
{
    QVariantMap map;
    return map;
}

QString MprisPlayer::playbackStatus() const
{
    Player *player = activePlayer();
    if (!player) {
        return "Stopped";
    }

    if (player->state() == "playing") {
        return "Playing";
    } else if (player->state() == "paused") {
        return "Paused";
    } else {
        return "Stopped";
    }
}

void MprisPlayer::Next()
{
    Player *player = activePlayer();
    if (!player) {
        return;
    }

    player->skipNext();
}

void MprisPlayer::Previous()
{
    Player *player = activePlayer();
    if (!player) {
        return;
    }

    player->skipPrevious();
}

void MprisPlayer::Pause()
{
    Player *player = activePlayer();
    if (!player || player->state() != "playing") {
        return;
    }

    player->playPause();
}

void MprisPlayer::PlayPause()
{
    Player *player = activePlayer();
    if (!player) {
        return;
    }

    player->playPause();
}

void MprisPlayer::Play()
{
    Player *player = activePlayer();
    if (!player || player->state() == "playing") {
        return;
    }

    player->playPause();
}

void MprisPlayer::Stop()
{
    Player *player = activePlayer();
    if (!player) {
        return;
    }
    player->stop();
}

/*void MprisPlayer::Seek(int64_t offset)
{

}*/

/*void MprisPlayer::SetPosition(QDBusObjectPath path, int64_t position)
{

}*/

/*void MprisPlayer::OpenUri(QString uri)
{

}*/

Player *MprisPlayer::activePlayer() const
{
    return Kodi::instance()->activePlayer();
}
