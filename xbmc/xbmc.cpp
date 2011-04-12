#include "xbmc.h"
#include "xbmcconnection.h"

#include "playlist.h"
#include "audiolibrary.h"

#include <QDebug>

namespace Xbmc
{

Xbmc::Xbmc(QObject *parent):
    QObject(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(parseAnnouncement(QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariant)), SLOT(responseReceived(int,QVariant)));

    int id = XbmcConnection::sendCommand("Player.GetActivePlayers");
    m_requestMap.insert(id, RequestActivePlayer);

    id = XbmcConnection::sendCommand("XBMC.GetVolume");
    m_requestMap.insert(id, RequestVolume);

    m_audioPlayer = new AudioPlayer(this);
    m_videoPlayer = new VideoPlayer(this);
    m_activePlayer = 0;

    m_audioLibrary = new AudioLibrary(m_audioPlayer, this);
}

QString Xbmc::state()
{
    if(m_activePlayer == m_audioPlayer) {
        return "audio";
    }
    if(m_activePlayer == m_videoPlayer) {
        return "video";
    }
    return "undefined";
}

AudioPlayer *Xbmc::audioPlayer()
{
    return m_audioPlayer;
}

AudioLibrary *Xbmc::audioLibrary()
{
    return m_audioLibrary;
}

VideoPlayer *Xbmc::videoPlayer()
{
    return m_videoPlayer;
}

Player *Xbmc::activePlayer()
{
    return m_activePlayer;
}

int Xbmc::volume()
{
    return m_volume;
}

void Xbmc::setVolume(int volume)
{
    if(volume != m_volume) {
        QVariantMap map;
        map.insert("value", volume);
        int id = XbmcConnection::sendCommand("XBMC.SetVolume", map);
        m_volume = volume;
//        m_requestMap.insert(id, RequestVolume);
    }
}

void Xbmc::toggleMute()
{

}

void Xbmc::parseAnnouncement(const QVariantMap &map)
{
    if(map.value("message").toString() == "PlaybackStarted") {
        int id = XbmcConnection::sendCommand("Player.GetActivePlayers");
        m_requestMap.insert(id, RequestActivePlayer);
    }
}

void Xbmc::responseReceived(int id, const QVariant &rsp)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    Player *activePlayer = 0;

    switch(m_requestMap.value(id)) {
    case RequestActivePlayer: {
//        qDebug() << "active player response:" << rsp;
        QVariantMap activePlayerMap = rsp.toMap();
        if(activePlayerMap.value("audio").toBool() == true) {
            activePlayer = m_audioPlayer;
        } else if(activePlayerMap.value("video").toBool() == true) {
            activePlayer = m_videoPlayer;
        } else if(activePlayerMap.value("pictures").toBool() == true) {
//            activePlayer = m_picturePlayer;
        }
        if(m_activePlayer != activePlayer) {
            m_activePlayer = activePlayer;
//            qDebug() << "active player changed!";
            emit stateChanged();
            emit activePlayerChanged(m_activePlayer);
        }
        if(m_activePlayer) {
            m_activePlayer->playlist()->refresh();
        }
        }
        break;
    case RequestVolume:
        m_volume = rsp.toInt();
//        qDebug() << "Volume received" << m_volume;
        emit volumeChanged(m_volume);
        break;

    }

}
QString Xbmc::vfsPath()
{
    return XbmcConnection::vfsPath();
}

}
