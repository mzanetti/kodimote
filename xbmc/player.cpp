/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
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

#include "xbmcconnection.h"
#include "xbmc.h"

#include "xdebug.h"

#include <QTime>

Player::Player(PlayerType type, QObject *parent) :
    QObject(parent),
    m_type(type),
    m_state("stopped"),
    m_speed(1),
    m_percentage(0),
    m_currentItem(new LibraryItem()),
    m_shuffle(false),
    m_repeat(RepeatNone)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    staticMetaObject.invokeMethod(this, "getSpeed", Qt::QueuedConnection);
    staticMetaObject.invokeMethod(this, "getPercentage", Qt::QueuedConnection);
    staticMetaObject.invokeMethod(this, "getPosition", Qt::QueuedConnection);
    staticMetaObject.invokeMethod(this, "getRepeatShuffle", Qt::QueuedConnection);

    m_percentageTimer.setInterval(1000);
    connect(&m_percentageTimer, SIGNAL(timeout()), SLOT(setPercentage()));
}

void Player::getSpeed()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    QVariantList properties;
    properties.append("speed");
    params.insert("properties", properties);
    int id = XbmcConnection::sendCommand("Player.GetProperties", params);
    m_requestMap.insert(id, RequestSpeed);
}

void Player::getPercentage()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    QVariantList props;
    props.append("percentage");
    params.insert("properties", props);
    int id = XbmcConnection::sendCommand("Player.GetProperties", params);
    m_requestMap.insert(id, RequestPercentage);
}

void Player::getPosition()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    QVariantList props;
    props.append("position");
    params.insert("properties", props);
    int id = XbmcConnection::sendCommand("Player.GetProperties", params);
    m_requestMap.insert(id, RequestPosition);
}

void Player::getRepeatShuffle()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    QVariantList props;
    props.append("repeat");
    props.append("shuffled");
    params.insert("properties", props);
    int id = XbmcConnection::sendCommand("Player.GetProperties", params);
    m_requestMap.insert(id, RequestRepeatShuffle);
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

    properties.append("showtitle");
//    properties.append("style");
//    properties.append("mood");
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

    int id = XbmcConnection::sendCommand("Player.GetItem", params);
    m_requestMap.insert(id, RequestCurrentItemDetails);
}

void Player::refresh()
{
    xDebug(XDAREA_PLAYER) << "player" << playerId() << "refreshing";
    getSpeed();
    getPercentage();
    getPosition();
    getRepeatShuffle();
    getCurrentItemDetails();
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

    XbmcConnection::sendCommand("Player.PlayPause", params);
}

void Player::stop()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    XbmcConnection::sendCommand("Player.Stop", params);
}

void Player::skipPrevious()
{
    QVariantMap params;
    params.insert("playerid", playerId());

    XbmcConnection::sendCommand("Player.GoPrevious", params);
}

void Player::skipNext()
{
    QVariantMap params;
    params.insert("playerid", playerId());

    XbmcConnection::sendCommand("Player.GoNext", params);
}

void Player::seekBackward()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("speed", "decrement");
    XbmcConnection::sendCommand("Player.SetSpeed", params);
}

void Player::seekForward()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("speed", "increment");
    XbmcConnection::sendCommand("Player.SetSpeed", params);
}

QString Player::state() const
{
    return m_state;
}

void Player::receivedAnnouncement(const QVariantMap &map)
{
    QVariantMap data= map.value("params").toMap().value("data").toMap();
    if(data.value("player").toMap().value("playerid").toInt() != playerId()) {
        return;
    }

    xDebug(XDAREA_PLAYER) << "Player" << playerId() << "got announcement:" << map.value("method") << data;

    if(map.value("method").toString() == "Player.OnStop") {
        xDebug(XDAREA_PLAYER) << "stopping player";
        m_state = "stopped";
        m_percentageTimer.stop();
        emit stateChanged();
        m_speed = 1;
        emit speedChanged();
        playlist()->refresh();
    } else if(map.value("method").toString() == "Player.OnPause") {
        m_state = "paused";
        m_percentageTimer.stop();
        emit stateChanged();
        m_speed = 1;
        emit speedChanged();
    } else if(map.value("method").toString() == "Player.OnPlay") {
        m_state = "playing";
        emit stateChanged();
        m_speed = data.value("player").toMap().value("speed").toInt();
        qDebug() << "set speed to" << m_speed;
        emit speedChanged();
        getPosition();
        getPercentage();
        getCurrentItemDetails();
        m_percentageTimer.start();
    } else if(map.value("method").toString() == "Player.OnSeek") {
        getPercentage();
    }
}

void Player::responseReceived(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestSpeed:
        m_speed = rsp.toMap().value("speed").toInt();
        xDebug(XDAREA_PLAYER) << "got player speed" << m_speed;
        emit speedChanged();

        if(m_speed == 0) {
            m_state = "paused";
            m_percentageTimer.stop();
        } else {
            m_state = "playing";
            getPercentage();
            m_percentageTimer.start();
        }
        emit stateChanged();


        break;
    case RequestPercentage:
        xDebug(XDAREA_PLAYER) << "Got percentage response" << response;
        m_percentage = rsp.toMap().value("percentage").toDouble();
        emit percentageChanged();
        break;
    case RequestPosition:
        xDebug(XDAREA_PLAYER) << "Got position response" << response;
        playlist()->setCurrentIndex(rsp.toMap().value("position").toInt());
        break;
    case RequestRepeatShuffle:
        if(rsp.toMap().value("repeat").toString() == "off") {
            m_repeat = RepeatNone;
        } else if(rsp.toMap().value("repeat").toString() == "one") {
            m_repeat = RepeatOne;
        } else {
            m_repeat = RepeatAll;
        }
        emit repeatChanged();

        m_shuffle = rsp.toMap().value("shuffled").toBool();
        emit shuffleChanged();
        break;
    case SetPercentage:
        m_requestMap.remove(id);
        break;
    case RequestCurrentItemDetails:
        xDebug(XDAREA_PLAYER) << "got current item details:" << rsp;
        QVariantMap itemMap = rsp.toMap().value("item").toMap();

        if(m_currentItem) {
            m_currentItem->deleteLater();
        }
        m_currentItem = new LibraryItem();
        m_currentItem->setType(itemMap.value("type").toString());
        m_currentItem->setTitle(itemMap.value("label").toString());
        if(itemMap.value("type").toString() == "song") {
            m_currentItem->setSubtitle(itemMap.value("artist").toString());
        } else if(itemMap.value("type").toString() == "episode") {
            m_currentItem->setSubtitle(itemMap.value("showtitle").toString());
        }

        m_currentItem->setComment(itemMap.value("comment").toString());
        m_currentItem->setGenre(itemMap.value("genre").toString());
        m_currentItem->setSeason(itemMap.value("season", -1).toInt());
        m_currentItem->setRating(itemMap.value("rating", -1).toInt());
        m_currentItem->setEpisode(itemMap.value("episode", -1).toInt());
        m_currentItem->setYear(itemMap.value("year").toString());
        m_currentItem->setDirector(itemMap.value("director").toString());
        m_currentItem->setTagline(itemMap.value("tagline").toString());
        m_currentItem->setMpaa(itemMap.value("mpaa").toString());
        m_currentItem->setInstrument(itemMap.value("instrument").toString());
        m_currentItem->setStyle(itemMap.value("style").toString());
        m_currentItem->setMood(itemMap.value("mood").toString());
        m_currentItem->setBorn(itemMap.value("born").toString());
        m_currentItem->setFormed(itemMap.value("formed").toString());
        m_currentItem->setDied(itemMap.value("died").toString());
        m_currentItem->setDisbanded(itemMap.value("disbanded").toString());
        if(itemMap.contains("runtime")) {
            m_currentItem->setDuration(QTime().addSecs(itemMap.value("runtime").toInt() * 60));
        } else  {
            m_currentItem->setDuration(QTime().addSecs(itemMap.value("duration").toInt()));
        }
        m_currentItem->setPlaycount(itemMap.value("playcount", -1).toInt());
        m_currentItem->setCast(itemMap.value("cast").toString());
        m_currentItem->setPlot(itemMap.value("plot").toString());
        m_currentItem->setThumbnail(itemMap.value("thumbnail").toString());
        m_currentItem->setAlbum(itemMap.value("album").toString());
        m_currentItem->setArtist(itemMap.value("artist").toString());
        m_currentItem->setFanart(itemMap.value("fanart").toString());
        m_currentItem->setTvShow(itemMap.value("showtitle").toString());
        emit currentItemChanged();
        break;
    }
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
    PlaylistItem *item = playlist()->currentItem();
    if(item) {
        QTime time;
        int currentTime = time.secsTo(playlist()->currentItem()->duration());
        currentTime = currentTime * m_percentage / 100;
        time = QTime();
        time = time.addSecs(currentTime);
        if(item->duration().hour() > 0) {
            return time.toString("hh:mm:ss");
        } else {
            return time.toString("mm:ss");
        }
    }
    return "00:00";
}

void Player::setPercentage()
{
    xDebug(XDAREA_PLAYER) << "setting percentage";
    if(!playlist()->currentItem()) {
        return;
    }
    QTime time;
    int duration = time.secsTo(playlist()->currentItem()->duration());
    if(duration > 0) {
        m_percentage += 100.0 / duration * m_speed;
    } else {
        m_percentage = 100;
    }
    emit percentageChanged();
    emit timeChanged();
}

void Player::playItem(int index)
{
    QVariantMap params;
    QVariantMap playlistid;
    playlistid.insertMulti("playlistid", playlist()->playlistId());
    playlistid.insert("position", index);
    params.insert("item", playlistid);
    XbmcConnection::sendCommand("Player.Open", params);
}

bool Player::shuffle() const
{
    return m_shuffle;
}

void Player::setShuffle(bool shuffle)
{
    QVariantMap params;
    params.insert("playerid", playerId());

    if(shuffle) {
        XbmcConnection::sendCommand("Player.Shuffle", params);
    } else {
        XbmcConnection::sendCommand("Player.UnShuffle", params);
    }
    getRepeatShuffle();
}

void Player::setRepeat(Player::Repeat repeat)
{
    QVariantMap params;
    switch(repeat) {
    case RepeatNone:
        params.insert("state", "off");
        break;
    case RepeatOne:
        params.insert("state", "one");
        break;
    case RepeatAll:
        params.insert("state", "all");
        break;
    }
    params.insert("playerid", playerId());

    XbmcConnection::sendCommand("Player.Repeat", params);

    getRepeatShuffle();
}

Player::Repeat Player::repeat() const
{
    return m_repeat;
}

void Player::seek(int position)
{
    if(!m_requestMap.keys(SetPercentage).isEmpty() && position != m_percentage) {
        return;
    }
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("value", position);

    m_requestMap.insert(XbmcConnection::sendCommand("Player.Seek", params), SetPercentage);
}

LibraryItem *Player::currentItem() const
{
    return m_currentItem;
}
