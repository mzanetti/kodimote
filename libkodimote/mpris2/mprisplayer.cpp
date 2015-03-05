#include "mprisplayer.h"

#include "kodi.h"
#include "libraryitem.h"
#include "player.h"
#include "playlist.h"

MprisPlayer::MprisPlayer(ProtocolManager *protocols, QObject *parent) :
    QDBusAbstractAdaptor(parent),
    m_player(Kodi::instance()->activePlayer()),
    m_protocols(protocols)
{
    setAutoRelaySignals(false);
    connect(Kodi::instance(), SIGNAL(activePlayerChanged()), SLOT(activePlayerChanged()));
    connect(Kodi::instance(), SIGNAL(volumeChanged(int)), SLOT(volumeChanged()));
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

        QString thumbnail = item->thumbnail();
        if (thumbnail == "loading") {
            //prevent connecting multiple times
            disconnect(item, SIGNAL(thumbnailChanged()), this, SLOT(thumbnailChanged()));
            connect(item, SIGNAL(thumbnailChanged()), this, SLOT(thumbnailChanged()));
        } else if (!thumbnail.isEmpty()) {
            map["mpris:artUrl"] = "file://" + item->thumbnail();
        }
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

double MprisPlayer::volume() const
{
    return Kodi::instance()->volume() / 100.0;
}

void MprisPlayer::setVolume(double volume)
{
    Kodi::instance()->setVolume(volume * 100);
}

bool MprisPlayer::shuffle() const
{
    if (!m_player) {
        return false;
    }

    return m_player->shuffle();
}

void MprisPlayer::setShuffle(bool shuffle)
{
    if (!m_player) {
        return;
    }

    m_player->setShuffle(shuffle);
}

QString MprisPlayer::loopStatus() const
{
    if (!m_player) {
        return "None";
    }

    switch (m_player->repeat()) {
    case Player::RepeatAll:
        return "Playlist";
    case Player::RepeatOne:
        return "Track";
    default:
        return "None";
    }
}

void MprisPlayer::setLoopStatus(QString loopStatus)
{
    if (!m_player) {
        return;
    }

    if (loopStatus == "Playlist") {
        m_player->setRepeat(Player::RepeatAll);
    } else if (loopStatus == "Track") {
        m_player->setRepeat(Player::RepeatOne);
    } else {
        m_player->setRepeat(Player::RepeatNone);
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

void MprisPlayer::OpenUri(QString uri)
{
    m_protocols->execute(uri);
}

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

void MprisPlayer::sendPropertyChanged(const QString &property)
{
    sendPropertiesChanged(QStringList(property));
}

void MprisPlayer::sendPropertiesChanged(const QStringList &properties)
{
    QDBusMessage signal = QDBusMessage::createSignal("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "PropertiesChanged");
    signal << "org.mpris.MediaPlayer2.Player";
    QVariantMap changedProps;
    foreach (QString property, properties) {
        changedProps.insert(property, this->property(property.toUtf8()));
    }
    signal << changedProps;
    signal << QStringList();
    QDBusConnection::sessionBus().send(signal);
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
        disconnect(m_player, SIGNAL(shuffleChanged()), this, SLOT(shuffleChanged()));
        disconnect(m_player, SIGNAL(repeatChanged()), this, SLOT(repeatChanged()));
        disconnect(m_player->playlist(), SIGNAL(countChanged()), this, SLOT(playlistChanged()));
        if (m_player->currentItem()) {
            disconnect(m_player->currentItem(), SIGNAL(thumbnailChanged()), this, SLOT(thumbnailChanged()));
        }
    }

    m_player = Kodi::instance()->activePlayer();

    if (m_player) {
        connect(m_player, SIGNAL(stateChanged()), this, SLOT(stateChanged()));
        connect(m_player, SIGNAL(currentItemChanged()), this, SLOT(currentItemChanged()));
        connect(m_player, SIGNAL(timeChanged()), this, SLOT(timeChanged()));
        connect(m_player, SIGNAL(speedChanged()), this, SLOT(speedChanged()));
        connect(m_player, SIGNAL(shuffleChanged()), this, SLOT(shuffleChanged()));
        connect(m_player, SIGNAL(repeatChanged()), this, SLOT(repeatChanged()));
        connect(m_player->playlist(), SIGNAL(countChanged()), this, SLOT(playlistChanged()));
    }

    currentItemChanged();
}

void MprisPlayer::volumeChanged()
{
    sendPropertyChanged("Volume");
}

void MprisPlayer::stateChanged()
{
    sendPropertyChanged("PlaybackStatus");
}

void MprisPlayer::currentItemChanged()
{
    connect(m_player->currentItem(), SIGNAL(thumbnailChanged()), this, SLOT(thumbnailChanged()));
    sendPropertiesChanged(QStringList() << "CanGoNext" << "CanGoPrevious" << "CanPause"
        << "CanPlay" << "CanSeek" << "Metadata" << "PlaybackStatus" << "Position" << "Rate");
}

void MprisPlayer::timeChanged()
{
    emit Seeked(QTime(0, 0, 0).msecsTo(m_player->time()) * Q_INT64_C(1000));
}

void MprisPlayer::speedChanged()
{
    sendPropertyChanged("Rate");
}

void MprisPlayer::shuffleChanged()
{
    sendPropertyChanged("Shuffle");
}

void MprisPlayer::repeatChanged()
{
    sendPropertyChanged("LoopStatus");
}

void MprisPlayer::playlistChanged()
{
    sendPropertiesChanged(QStringList() << "CanGoNext" << "CanGoPrevious");
}

void MprisPlayer::thumbnailChanged()
{
    sendPropertyChanged("Metadata");
}
