/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "player.h"
#include "playlist.h"
#include "libraryitem.h"

#include "kodiconnection.h"
#include "kodi.h"

#include "kodebug.h"

#include <QTime>

Player::Player(PlayerType type, QObject *parent) :
    QObject(parent),
    m_type(type),
    m_state("stopped"),
    m_speed(1),
    m_percentage(0),
    m_lastPlaytime(0),
    m_lastPlaytimeUpdate(QDateTime::currentDateTime()),
    m_currentItem(new LibraryItem(this)),
    m_seeking(false),
    m_timerActivated(false),
    m_shuffle(false),
    m_repeat(RepeatNone),
    m_currentSubtitle(-1),
    m_currentAudiostream(0)
{
    qDebug() << "player created libraryItem" << m_currentItem << m_currentItem->rating();
    connect(KodiConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));

    m_playtimeTimer.setInterval(1000);
    connect(&m_playtimeTimer, SIGNAL(timeout()), SLOT(updatePlaytime()));
}

void Player::getRepeatShuffle()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    QVariantList props;
    props.append("repeat");
    props.append("shuffled");
    params.insert("properties", props);
    KodiConnection::sendCommand("Player.GetProperties", params, this, "repeatShuffleReceived");
}

void Player::getMediaProps()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    QVariantList props;
    props.append("subtitles");
    props.append("currentsubtitle");
    props.append("subtitleenabled");
    props.append("audiostreams");
    props.append("currentaudiostream");
    props.append("position");
    params.insert("properties", props);
    KodiConnection::sendCommand("Player.GetProperties", params, this, "mediaPropsReceived");
}

void Player::getCurrentItemDetails()
{
    QVariantMap params;
    params.insert("playerid", playerId());

    QVariantList properties;
    properties.append("title");
    properties.append("artist");
    properties.append("comment");
    properties.append("genre");
    properties.append("season");
    properties.append("rating");
    properties.append("episode");
    properties.append("year");
    properties.append("director");
    properties.append("tagline");
    properties.append("mpaa");
    properties.append("album");
    properties.append("fanart");
    properties.append("file");

    properties.append("showtitle");
    properties.append("style");
    properties.append("mood");
//    properties.append("born");
//    properties.append("formed");
//    properties.append("died");
//    properties.append("disbanded");
    properties.append("duration");
    properties.append("playcount");
    properties.append("cast");
    properties.append("plot");
//    properties.append("description");
    properties.append("thumbnail");
    properties.append("runtime");
    params.insert("properties", properties);

    KodiConnection::sendCommand("Player.GetItem", params, this, "detailsReceived");
}

void Player::refresh()
{
    koDebug(XDAREA_PLAYER) << "player" << playerId() << "refreshing";
    QVariantMap params;
    params.insert("playerid", playerId());
    QVariantList props;
    props.append("speed");
    props.append("time");
    props.append("totaltime");
    props.append("position");
    props.append("repeat");
    props.append("shuffled");
    props.append("subtitles");
    props.append("currentsubtitle");
    props.append("audiostreams");
    props.append("currentaudiostream");
    params.insert("properties", props);
    KodiConnection::sendCommand("Player.GetProperties", params, this, "refreshReceived");
    getCurrentItemDetails();
    playlist()->refresh();
}

void Player::detach()
{
    koDebug(XDAREA_PLAYER) << "stopping player";
    m_state = "stopped";
    m_playtimeTimer.stop();
    emit stateChanged();
    m_speed = 1;
    emit speedChanged();
    playlist()->refresh();
}

Player::PlayerType Player::type() const
{
    return m_type;
}


void Player::playPause()
{
    QVariantMap params;
    params.insert("playerid", playerId());

    KodiConnection::sendCommand("Player.PlayPause", params);
}

void Player::stop()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    KodiConnection::sendCommand("Player.Stop", params);
}

void Player::skipPrevious()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("to", "previous");

    KodiConnection::sendCommand("Player.GoTo", params);
}

void Player::skipNext()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("to", "next");

    KodiConnection::sendCommand("Player.GoTo", params);
}

void Player::seekBackward()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("speed", "decrement");
    KodiConnection::sendCommand("Player.SetSpeed", params);
}

void Player::seekForward()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("speed", "increment");
    KodiConnection::sendCommand("Player.SetSpeed", params);
}

QString Player::state() const
{
    return m_state;
}

void Player::receivedAnnouncement(const QVariantMap &map)
{
    QVariantMap data= map.value("params").toMap().value("data").toMap();
    if(!data.value("player").isValid() || data.value("player").toMap().value("playerid").toInt() != playerId()) {
        return;
    }

    koDebug(XDAREA_PLAYER) << "Player" << playerId() << "got announcement:" << map.value("method") << data;

    if(map.value("method").toString() == "Player.OnStop") {
        //is most likely unreachable as Player.OnStop doesn't contain a player id
        // and thus it would already return above
        detach();
    } else if(map.value("method").toString() == "Player.OnPause") {
        m_state = "paused";
        m_playtimeTimer.stop();
        updatePlaytime();
        emit stateChanged();
        m_speed = 1;
        emit speedChanged();
    } else if(map.value("method").toString() == "Player.OnPlay") {
        m_state = "playing";
        emit stateChanged();
        //this has to be delayed, otherwise returned times aren't correct (yet)
        QTimer::singleShot(100, this, SLOT(refresh()));
        m_speed = data.value("player").toMap().value("speed").toInt();
        emit speedChanged();
    } else if(map.value("method").toString() == "Player.OnSeek") {
        updatePlaytime(data.value("player").toMap().value("time").toMap());
        m_seeking = false;
    } else if(map.value("method").toString() == "Player.OnSpeedChanged") {
        updatePlaytime();
        m_speed = data.value("player").toMap().value("speed").toInt();
        emit speedChanged();
    }
}

void Player::repeatShuffleReceived(const QVariantMap &rsp)
{
    QVariant result = rsp.value("result");
    if(result.toMap().value("repeat").toString() == "off") {
        m_repeat = RepeatNone;
    } else if(result.toMap().value("repeat").toString() == "one") {
        m_repeat = RepeatOne;
    } else {
        m_repeat = RepeatAll;
    }
    emit repeatChanged();

    m_shuffle = result.toMap().value("shuffled").toBool();
    emit shuffleChanged();
}

void Player::mediaPropsReceived(const QVariantMap &rsp)
{
    QVariant result = rsp.value("result");

    QVariantList subtitleList = result.toMap().value("subtitles").toList();
    m_subtitles.clear();
    foreach (const QVariant &sub, subtitleList) {
        QString label;
        if (!sub.toMap().value("language").toString().isEmpty()) {
            label = sub.toMap().value("language").toString();
        } else {
            label = sub.toMap().value("name").toString();
        }
        m_subtitles.append(label);
    }

    m_currentSubtitle = result.toMap().value("currentsubtitle").toMap().value("index").toInt();
    if (!result.toMap().value("subtitleenabled").toBool()) {
        m_currentSubtitle = -1;
    }

    koDebug(XDAREA_PLAYER) << "got subtitles:" << m_subtitles;
    emit subtitlesChanged();
    emit currentSubtitleChanged();

    QVariantList audiostreamList = result.toMap().value("audiostreams").toList();
    m_audiostreams.clear();
    foreach (const QVariant &as, audiostreamList) {
        QString label = as.toMap().value("name").toString();
        if (!as.toMap().value("language").toString().isEmpty()) {
            label += " - " + as.toMap().value("language").toString();
        }
        m_audiostreams.append(label);
    }

    m_currentAudiostream = result.toMap().value("currentaudiostream").toMap().value("index").toInt();

    koDebug(XDAREA_PLAYER) << "got audiostreams:" << m_subtitles;
    emit audiostreamsChanged();
    emit currentAudiostreamChanged();
}

void Player::refreshReceived(const QVariantMap &rsp)
{
    repeatShuffleReceived(rsp);
    mediaPropsReceived(rsp);

    QVariantMap result = rsp.value("result").toMap();
    m_totalTime = parseTime(result.value("totaltime").toMap());

    m_speed = result.value("speed").toInt();

    if(m_speed == 0) {
        m_state = "paused";
    } else {
        m_state = "playing";
    }
    emit stateChanged();

    updatePlaytime(result.value("time").toMap());
    if (m_timerActivated && !m_playtimeTimer.isActive()) {
        m_playtimeTimer.start();
    }

    playlist()->setCurrentIndex(result.value("position").toInt());
}

void Player::detailsReceived(const QVariantMap &rsp)
{
    koDebug(XDAREA_PLAYER) << "got current item details:" << rsp;
    QVariantMap itemMap = rsp.value("result").toMap().value("item").toMap();

    if(m_currentItem) {
        m_currentItem->deleteLater();
    }
    m_currentItem = new LibraryItem(this);

    int id = itemMap.value("id").toInt();
    QString type = itemMap.value("type").toString();
    if(type == "episode") {
        m_currentItem->setEpisodeId(id);
    } else if(type == "movie") {
        m_currentItem->setMovieId(id);
    } else if(type == "musicvideo") {
        m_currentItem->setMusicvideoId(id);
    } else if(type == "song") {
        m_currentItem->setSongId(id);
    } else if(type == "album") {
        m_currentItem->setAlbumId(id);
    } else if(type == "artist") {
        m_currentItem->setArtistId(id);
    } else if(type == "channel") {
        m_currentItem->setChannelId(id);
    }

    m_currentItem->setType(type);


    m_currentItem->setTitle(itemMap.value("label").toString());
    if(itemMap.value("type").toString() == "song") {
        m_currentItem->setSubtitle(itemMap.value("artist").toString());
    } else if(itemMap.value("type").toString() == "episode") {
        m_currentItem->setSubtitle(itemMap.value("showtitle").toString());
    } else if(itemMap.value("type").toString() == "channel") {
        m_currentItem->setTitle(itemMap.value("title").toString());
        m_currentItem->setSubtitle(itemMap.value("label").toString());
    }

    m_currentItem->setComment(itemMap.value("comment").toString());
    m_currentItem->setGenre(itemMap.value("genre").toString());
    if (m_currentItem->genre().isEmpty() && itemMap.value("genre").toList().count() > 0) {
        m_currentItem->setGenre(itemMap.value("genre").toStringList().join("/"));
    }
    m_currentItem->setSeason(itemMap.value("season", -1).toInt());
    m_currentItem->setRating(itemMap.value("rating", -1).toInt());
    qDebug() << "set rating to" << m_currentItem->rating();
    m_currentItem->setEpisode(itemMap.value("episode", -1).toInt());
    m_currentItem->setYear(itemMap.value("year").toString());
    m_currentItem->setDirector(itemMap.value("director").toString());
    m_currentItem->setTagline(itemMap.value("tagline").toString());
    m_currentItem->setMpaa(itemMap.value("mpaa").toString());
    m_currentItem->setInstrument(itemMap.value("instrument").toString());
    m_currentItem->setStyle(itemMap.value("style").toStringList().join("/"));
    m_currentItem->setMood(itemMap.value("mood").toStringList().join("/"));
    m_currentItem->setBorn(itemMap.value("born").toString());
    m_currentItem->setFormed(itemMap.value("formed").toString());
    m_currentItem->setDied(itemMap.value("died").toString());
    m_currentItem->setDisbanded(itemMap.value("disbanded").toString());
    if(itemMap.contains("runtime")) {
        m_currentItem->setDuration(QTime(0, 0, 0).addSecs(itemMap.value("runtime").toDouble()));
        qDebug() << "set runtime 1" << m_currentItem->duration() << itemMap.value("runtime");
    } else  {
        m_currentItem->setDuration(QTime(0, 0, 0).addSecs(itemMap.value("duration").toInt()));
        qDebug() << "set runtime 2" << m_currentItem->duration();
    }
    m_currentItem->setPlaycount(itemMap.value("playcount", -1).toInt());
    m_currentItem->setCast(itemMap.value("cast").toString());
    m_currentItem->setPlot(itemMap.value("plot").toString());
    m_currentItem->setThumbnail(itemMap.value("thumbnail").toString());
    m_currentItem->setAlbum(itemMap.value("album").toString());
    m_currentItem->setArtist(itemMap.value("artist").toString());
    if (m_currentItem->artist().isEmpty() && itemMap.value("artist").toList().count() > 0) {
        m_currentItem->setArtist(itemMap.value("artist").toStringList().join("/"));
    }
    m_currentItem->setFanart(itemMap.value("fanart").toString());
    m_currentItem->setTvShow(itemMap.value("showtitle").toString());
    m_currentItem->setFileName(itemMap.value("file").toString());
    emit currentItemChanged();
}

int Player::speed() const
{
    return m_speed;
}

double Player::percentage() const
{
    return m_percentage;
}

QString Player::time() const
{
    if(m_currentItem) {
        QTime time = QTime(0, 0, 0).addMSecs(m_lastPlaytime);
        if(m_currentItem->duration().hour() > 0) {
            return time.toString("hh:mm:ss");
        } else {
            return time.toString("mm:ss");
        }
    }
    return "00:00";
}

QTime Player::totalTime() const
{
    return m_totalTime;
}

QString Player::totalTimeString() const
{
    return formatTime(m_totalTime);
}

void Player::updatePlaytime()
{
    if(!m_currentItem) {
        return;
    }

    //use milliseconds, otherwise it tends to skip a sec. once in a while
    int duration = QTime(0, 0, 0).msecsTo(m_totalTime);
    QDateTime currentTime = QDateTime::currentDateTime();
    int elapsedMSeconds = m_lastPlaytimeUpdate.msecsTo(currentTime);
    m_lastPlaytime += elapsedMSeconds * m_speed;
    m_percentage = qMax(0.0, qMin(100.0, (double)m_lastPlaytime / duration * 100));
    m_lastPlaytimeUpdate = currentTime;

    emit percentageChanged();
    emit timeChanged();
}

void Player::updatePlaytime(const QVariantMap &timeMap)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QTime time = parseTime(timeMap);
    m_lastPlaytime = QTime(0, 0, 0).msecsTo(time);
    m_lastPlaytimeUpdate = currentTime;
    int duration = QTime(0, 0, 0).msecsTo(m_totalTime);
    m_percentage = (double)m_lastPlaytime / duration * 100;

    emit percentageChanged();
    emit timeChanged();
}

void Player::playItem(int index)
{
    QVariantMap params;
    QVariantMap playlistid;
    playlistid.insert("playlistid", playlist()->playlistId());
    playlistid.insert("position", index);
    params.insert("item", playlistid);
    KodiConnection::sendCommand("Player.Open", params);
}

void Player::open(const PlaylistItem &item, bool resume)
{
    QVariantMap params;
    params.insert("item", item.toMap());
    QVariantMap options;
    options.insert("resume", resume);
    params.insert("options", options);
    KodiConnection::sendCommand("Player.Open", params);
}

bool Player::shuffle() const
{
    return m_shuffle;
}

void Player::setShuffle(bool shuffle)
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("shuffle", shuffle);

    KodiConnection::sendCommand("Player.SetShuffle", params);
    getRepeatShuffle();
}

void Player::setRepeat(Player::Repeat repeat)
{
    QVariantMap params;
    switch(repeat) {
    case RepeatNone:
        params.insert("repeat", "off");
        break;
    case RepeatOne:
        params.insert("repeat", "one");
        break;
    case RepeatAll:
        params.insert("repeat", "all");
        break;
    }
    params.insert("playerid", playerId());

    KodiConnection::sendCommand("Player.SetRepeat", params);

    getRepeatShuffle();
}

QStringList Player::subtitles() const
{
    return m_subtitles;
}

int Player::currentSubtitle() const
{
    return m_currentSubtitle;
}

void Player::setCurrentSubtitle(int index)
{
    QVariantMap params;
    params.insert("playerid", playerId());

    if (index >= 0 && index < m_subtitles.count()) {
        // Select the stream we want
        params.insert("subtitle", index);
        KodiConnection::sendCommand("Player.SetSubtitle", params);

        // Enable in case they're off
        params.insert("subtitle", "on");
        KodiConnection::sendCommand("Player.SetSubtitle", params);

    } else {
        // Disable
        params.insert("subtitle", "off");
        KodiConnection::sendCommand("Player.SetSubtitle", params);
    }

    getMediaProps();
}

QStringList Player::audiostreams() const
{
    return m_audiostreams;
}

int Player::currentAudiostream() const
{
    return m_currentAudiostream;
}

void Player::setCurrentAudiostream(int index)
{
    if (index >= 0 && index < m_audiostreams.count()) {
        QVariantMap params;
        params.insert("playerid", playerId());
        params.insert("stream", index);
        KodiConnection::sendCommand("Player.SetAudioStream", params);
        getMediaProps();
    }
}

Player::Repeat Player::repeat() const
{
    return m_repeat;
}

bool Player::timerActive() const
{
    return m_playtimeTimer.isActive();
}

void Player::setTimerActive(bool active)
{
    if(m_playtimeTimer.isActive() == active) {
        return;
    }

    m_timerActivated = active;

    if(active) {
        if(m_state == "playing") {
            m_playtimeTimer.start();
            updatePlaytime();
        }
    }
    else {
        m_playtimeTimer.stop();
    }
}

void Player::seek(double percentage)
{
    if(m_seeking) {
        return;
    }

    QTime position = calculateTime(percentage);
    QVariantMap params;
    params.insert("playerid", playerId());
    QVariantMap time;
    time.insert("hours", position.hour());
    time.insert("minutes", position.minute());
    time.insert("seconds", position.second());
    params.insert("value", time);

    m_seeking = true;
    KodiConnection::sendCommand("Player.Seek", params);
}

LibraryItem *Player::currentItem() const
{
    return m_currentItem;
}

QTime Player::parseTime(const QVariantMap &timeMap) const
{
    QTime time;
    int hours = timeMap.value("hours").toInt();
    int minutes = timeMap.value("minutes").toInt();
    int seconds = timeMap.value("seconds").toInt();
    int mseconds = timeMap.value("milliseconds").toInt();
    time.setHMS(hours, minutes, seconds, mseconds);

    return time;
}

QString Player::formatTime(const QTime &time) const
{
    if (time.hour() > 0) {
        return time.toString("hh:mm:ss");
    } else {
        return time.toString("mm:ss");
    }
}

QTime Player::calculateTime(double percentage) const
{
    int milliSeconds = QTime(0, 0, 0).msecsTo(m_totalTime) * percentage / 100;
    return QTime(0, 0, 0).addMSecs(milliSeconds);
}

QString Player::calculateTimeString(double percentage) const
{
    return formatTime(calculateTime(percentage));
}
