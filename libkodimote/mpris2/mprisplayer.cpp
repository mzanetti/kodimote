#include "mprisplayer.h"

#include "kodi.h"
#include "player.h"
#include "playlist.h"

MprisPlayer::MprisPlayer(QObject *parent) :
    QDBusAbstractAdaptor(parent),
    m_player(Kodi::instance()->activePlayer())
{
    setAutoRelaySignals(false);
    connect(Kodi::instance(), SIGNAL(activePlayerChanged()), SLOT(activePlayerChanged()));
}

bool MprisPlayer::canControl() const
{
    return true;
}

bool MprisPlayer::canGoNext() const
{
    if (!m_player) {
        return false;
    }

    return m_player->playlist()->currentTrackNumber() < m_player->playlist()->count();
}

bool MprisPlayer::canGoPrevious() const
{
    if (!m_player) {
        return false;
    }

    return m_player->playlist()->currentTrackNumber() > 1;
}

bool MprisPlayer::canPause() const
{
    return true;
}

bool MprisPlayer::canPlay() const
{
    if (!m_player) {
        return false;
    }

    return !!m_player->currentItem();
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
    if (!m_player) {
        return "Stopped";
    }

    if (m_player->state() == "playing") {
        return "Playing";
    } else if (m_player->state() == "paused") {
        return "Paused";
    } else {
        return "Stopped";
    }
}

void MprisPlayer::Next()
{
    if (!m_player) {
        return;
    }

    m_player->skipNext();
}

void MprisPlayer::Previous()
{
    if (!m_player) {
        return;
    }

    m_player->skipPrevious();
}

void MprisPlayer::Pause()
{
    if (!m_player || m_player->state() != "playing") {
        return;
    }

    m_player->playPause();
}

void MprisPlayer::PlayPause()
{
    if (!m_player) {
        return;
    }

    m_player->playPause();
}

void MprisPlayer::Play()
{
    if (!m_player || m_player->state() == "playing") {
        return;
    }

    m_player->playPause();
}

void MprisPlayer::Stop()
{
    if (!m_player) {
        return;
    }
    m_player->stop();
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

void MprisPlayer::activePlayerChanged()
{
    if (m_player == Kodi::instance()->activePlayer()) {
        return;
    }

    if (m_player) {
        disconnect(m_player, 0, this, 0);
        disconnect(m_player->playlist(), SIGNAL(countChanged()), this, SLOT(playlistChanged()));
    }

    m_player = Kodi::instance()->activePlayer();

    if (m_player) {
        connect(m_player, SIGNAL(stateChanged()), this, SLOT(stateChanged()));
        connect(m_player, SIGNAL(currentItemChanged()), this, SLOT(currentItemChanged()));
        connect(m_player->playlist(), SIGNAL(countChanged()), this, SLOT(playlistChanged()));
    }

    currentItemChanged();
}

void MprisPlayer::stateChanged()
{
    QDBusMessage signal = QDBusMessage::createSignal("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "PropertiesChanged");
    signal << "org.mpris.MediaPlayer2.Player";
    QVariantMap changedProps;
    changedProps.insert("PlaybackStatus", playbackStatus());
    signal << changedProps;
    signal << QStringList();
    QDBusConnection::sessionBus().send(signal);
}

void MprisPlayer::currentItemChanged()
{
    QDBusMessage signal = QDBusMessage::createSignal("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "PropertiesChanged");
    signal << "org.mpris.MediaPlayer2.Player";
    QVariantMap changedProps;
    changedProps.insert("CanGoNext", canGoNext());
    changedProps.insert("CanGoPrevious", canGoPrevious());
    changedProps.insert("CanPause", canPause());
    changedProps.insert("CanPlay", canPlay());
    changedProps.insert("CanSeek", canSeek());
    changedProps.insert("Metadata", metadata());
    changedProps.insert("PlaybackStatus", playbackStatus());
    signal << changedProps;
    signal << QStringList();
    QDBusConnection::sessionBus().send(signal);
}

void MprisPlayer::playlistChanged()
{
    QDBusMessage signal = QDBusMessage::createSignal("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "PropertiesChanged");
    signal << "org.mpris.MediaPlayer2.Player";
    QVariantMap changedProps;
    changedProps.insert("CanGoNext", canGoNext());
    changedProps.insert("CanGoPrevious", canGoPrevious());
    signal << changedProps;
    signal << QStringList();
    QDBusConnection::sessionBus().send(signal);
}
