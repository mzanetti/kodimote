#include "xbmc.h"
#include "xbmcconnection.h"

#include "audiolibrary.h"
#include "artists.h"

#include "videolibrary.h"

#include "files.h"
#include "shares.h"

#include "playlist.h"
#include "audioplaylist.h"

#include "audioplayer.h"
#include "videoplayer.h"

#include "audioplaylistitem.h"
#include "videoplaylistitem.h"

#include "keys.h"

#include <QSettings>
#include <QtDeclarative>

Xbmc *Xbmc::s_instance = 0;

Xbmc *Xbmc::instance()
{
    if(!s_instance) {
        s_instance = new Xbmc();
    }
    return s_instance;
}

Xbmc::Xbmc(QObject *parent) :
    QObject(parent)
{
    qmlRegisterType<AudioLibrary>();
    qmlRegisterType<VideoLibrary>();
    qmlRegisterType<Artists>();
    qmlRegisterType<XbmcModel>();
    qmlRegisterType<Player>();
    qmlRegisterType<AudioPlayer>();
    qmlRegisterType<VideoPlayer>();
    qmlRegisterType<Playlist>();
    qmlRegisterType<PlaylistItem>();
    qmlRegisterType<AudioPlaylistItem>();
    qmlRegisterType<VideoPlaylistItem>();
    qmlRegisterType<Files>();
    qmlRegisterType<Shares>();
    qmlRegisterType<Keys>();

    qmlRegisterType<AudioPlaylist>("Xbmc", 1, 0, "Playlist");

    QSettings settings("xbmcremote");
    m_hostname = settings.value("Host").toString();
    m_port = settings.value("Port").toInt();

    XbmcConnection::connect(m_hostname, m_port);
//    XbmcConnection::connect("10.10.10.10", 8080);

    connect(XbmcConnection::notifier(), SIGNAL(connectionChanged()), SLOT(connectionChanged()));
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(parseAnnouncement(QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));

    int id = XbmcConnection::sendCommand("Player.GetActivePlayers");
    m_requestMap.insert(id, RequestActivePlayer);

    id = XbmcConnection::sendCommand("XBMC.GetVolume");
    m_requestMap.insert(id, RequestVolume);

    m_audioPlayer = new AudioPlayer(this);
    m_videoPlayer = new VideoPlayer(this);
    m_activePlayer = m_audioPlayer;
    m_state = "undefined";

    m_keys = new Keys(this);
}

Xbmc::~Xbmc()
{
}

bool Xbmc::connected()
{
    qDebug() << "****" << XbmcConnection::connected();
    return XbmcConnection::connected();
}

AudioLibrary *Xbmc::audioLibrary()
{
    return new AudioLibrary();
}

VideoLibrary *Xbmc::videoLibrary()
{
    return new VideoLibrary();
}

Shares *Xbmc::shares(const QString &mediatype)
{
    return new Shares(mediatype);
}

AudioPlayer *Xbmc::audioPlayer()
{
    return m_audioPlayer;
}

VideoPlayer *Xbmc::videoPlayer()
{
    return m_videoPlayer;
}

QString Xbmc::hostname()
{
    return m_hostname;
}

void Xbmc::setHostname(const QString &hostname)
{
    QSettings settings("xbmcremote");
    settings.setValue("Host", hostname);
    m_hostname = hostname;
    emit hostnameChanged();
}

int Xbmc::port(){
    qDebug() << "port" << m_port;
    return m_port;
}

void Xbmc::setPort(int port)
{
    QSettings settings("xbmcremote");
    settings.setValue("Port", port);
    m_port = port;
    emit portChanged();
}

void Xbmc::connectToHost()
{
    qDebug() << "fdfdsfsdafas";
    XbmcConnection::connect(m_hostname, m_port);
}

Player *Xbmc::activePlayer()
{
    return m_activePlayer;
}

void Xbmc::parseAnnouncement(const QVariantMap &map)
{
    qDebug() << "incoming announcement" << map << "method:" << map.value("method").toString();
    if(map.value("method").toString() == "Player.OnPlay") {
        int id = XbmcConnection::sendCommand("Player.GetActivePlayers");
        m_requestMap.insert(id, RequestActivePlayer);
    }
}

void Xbmc::responseReceived(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    Player *activePlayer = 0;

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestActivePlayer: {
//        qDebug() << "active player response:" << rsp;
        QVariantMap activePlayerMap = rsp.toMap();
        if(activePlayerMap.value("audio").toBool() == true) {
            activePlayer = m_audioPlayer;
            m_state = "audio";
        } else if(activePlayerMap.value("video").toBool() == true) {
            activePlayer = m_videoPlayer;
            m_state = "video";
        } else if(activePlayerMap.value("pictures").toBool() == true) {
//            activePlayer = m_picturePlayer;
        }
        if(m_activePlayer != activePlayer) {
            m_activePlayer = activePlayer;
            qDebug() << "active player changed!";
            emit stateChanged();
            emit activePlayerChanged();
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

QString Xbmc::state()
{
    return m_state;
}

Keys *Xbmc::keys()
{
    return m_keys;
}

void Xbmc::connectionChanged()
{
    if(connected()) {
        m_requestMap.insert(XbmcConnection::sendCommand("XBMC.GetVolume"), RequestVolume);
    }
    emit connectedChanged();
}

void Xbmc::setVolume(int volume)
{
    if(volume != m_volume) {
        QVariantMap map;
        map.insert("value", volume);
        XbmcConnection::sendCommand("XBMC.SetVolume", map);
        m_volume = volume;
        emit volumeChanged(m_volume);
    }
}

int Xbmc::volume()
{
    return m_volume;
}
