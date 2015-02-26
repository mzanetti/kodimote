#include "mprisplayer.h"

#include "kodi.h"
#include "libraryitem.h"
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
    return true;
}

double MprisPlayer::maximumRate() const
{
    return 32.0;
}

double MprisPlayer::minimumRate() const
{
    return -32.0;
}

QVariantMap MprisPlayer::metadata() const
{
    QVariantMap map;
    if (m_player && m_player->currentItem()) {
        LibraryItem *item = m_player->currentItem();
        map["mpris:trackid"] = qVariantFromValue(QDBusObjectPath(buildPath(item)));
        map["mpris:length"] = QTime(0, 0, 0).msecsTo(item->duration()) * Q_INT64_C(1000); // milliseconds vs microseconds
        map["xesam:title"] = item->title();
        map["xesam:artist"] = item->subtitle();
        map["xesam:album"] = item->album();
    }
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

qint64 MprisPlayer::position() const
{
    if (!m_player) {
        return 0;
    }

    m_player->updatePlaytime();
    QTime time = m_player->time();
    return QTime(0, 0, 0).msecsTo(time) * Q_INT64_C(1000); // milliseconds vs microseconds
}

double MprisPlayer::rate() const
{
    if (!m_player) {
        return 1.0;
    }

    return m_player->state() == "playing" ? m_player->speed() : 1.0;
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

void MprisPlayer::Seek(qint64 offset)
{
    if (!m_player || !m_player->currentItem()) {
        return;
    }

    int currentTime = QTime(0, 0, 0).msecsTo(m_player->time());
    int newTime = currentTime + (offset / 1000);
    if (newTime < 0) {
        newTime = 0;
    }

    QTime time = QTime(0, 0, 0).addMSecs(newTime);
    //TODO: use m_player->totalTime from player improvements branch
    if (time > m_player->currentItem()->duration()) {
        m_player->skipNext();
    } else {
        m_player->seek(time);
    }
}

void MprisPlayer::SetPosition(QDBusObjectPath path, qint64 position)
{
    if (!m_player || !m_player->currentItem()) {
        return;
    }

    if (buildPath(m_player->currentItem()) != path.path()) {
        return;
    }

    QTime time = QTime(0, 0, 0).addMSecs(position / 1000);
    m_player->seek(time);
}

/*void MprisPlayer::OpenUri(QString uri)
{

}*/

QString MprisPlayer::buildPath(LibraryItem *item) const
{
    if (item->episodeId() != -1) {
        return QString("/org/mpris/MediaPlayer2/Track/episode/%1").arg(item->episodeId());
    } else if (item->movieId() != -1) {
        return QString("/org/mpris/MediaPlayer2/Track/movie/%1").arg(item->movieId());
    } else if (item->musicvideoId() != -1) {
        return QString("/org/mpris/MediaPlayer2/Track/musicVideo/%1").arg(item->musicvideoId());
    } else if (item->channelId() != -1) {
        return QString("/org/mpris/MediaPlayer2/Track/channel/%1").arg(item->channelId());
    } else if (item->songId() != -1) {
        return QString("/org/mpris/MediaPlayer2/Track/song/%1").arg(item->songId());
    }

    return "";
}

void MprisPlayer::activePlayerChanged()
{
    if (m_player == Kodi::instance()->activePlayer()) {
        return;
    }

    if (m_player) {
        disconnect(m_player, SIGNAL(stateChanged()), this, SLOT(stateChanged()));
        disconnect(m_player, SIGNAL(currentItemChanged()), this, SLOT(currentItemChanged()));
        disconnect(m_player, SIGNAL(timeChanged()), this, SLOT(timeChanged()));
        disconnect(m_player, SIGNAL(speedChanged()), this, SLOT(speedChanged()));
        disconnect(m_player->playlist(), SIGNAL(countChanged()), this, SLOT(playlistChanged()));
    }

    m_player = Kodi::instance()->activePlayer();

    if (m_player) {
        connect(m_player, SIGNAL(stateChanged()), this, SLOT(stateChanged()));
        connect(m_player, SIGNAL(currentItemChanged()), this, SLOT(currentItemChanged()));
        connect(m_player, SIGNAL(timeChanged()), this, SLOT(timeChanged()));
        connect(m_player, SIGNAL(speedChanged()), this, SLOT(speedChanged()));
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
    changedProps.insert("Position", position());
    changedProps.insert("Rate", rate());
    signal << changedProps;
    signal << QStringList();
    QDBusConnection::sessionBus().send(signal);
}

void MprisPlayer::timeChanged()
{
    emit Seeked(QTime(0, 0, 0).msecsTo(m_player->time()) * Q_INT64_C(1000));
}

void MprisPlayer::speedChanged()
{
    QDBusMessage signal = QDBusMessage::createSignal("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "PropertiesChanged");
    signal << "org.mpris.MediaPlayer2.Player";
    QVariantMap changedProps;
    changedProps.insert("Rate", rate());
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
