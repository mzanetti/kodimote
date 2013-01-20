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

#include "xbmc.h"
#include "xbmchostmodel.h"
#include "xbmcfiltermodel.h"
#include "xbmcconnection.h"
#include "xdebug.h"
#include "xbmcdiscovery.h"
#include "xbmcdownload.h"

#include "audiolibrary.h"
#include "artists.h"

#include "videolibrary.h"

#include "files.h"
#include "shares.h"
#include "channelgroups.h"
#include "channels.h"

#include "playlist.h"
#include "audioplaylist.h"

#include "audioplayer.h"
#include "videoplayer.h"
#include "pictureplayer.h"

#include "libraryitem.h"
#include "audioplaylistitem.h"
#include "videoplaylistitem.h"

#include "keys.h"
#include "eventclient.h"

//#include "settings.h"

#include "imagecache.h"

#include <QSettings>

#ifdef QT5_BUILD
#include <QtQuick>
#else
#include <QtDeclarative>
#endif

Xbmc *Xbmc::s_instance = 0;

Xbmc *Xbmc::instance()
{
    if(!s_instance) {
        s_instance = new Xbmc();
    }
    return s_instance;
}

Xbmc::Xbmc(QObject *parent) :
    QObject(parent),
    m_volume(0),
    m_canShutdown(false),
    m_canReboot(false),
    m_canHibernate(false),
    m_canSuspend(false),
    m_pvrAvailable(false),
    m_pvrRecording(false),
    m_pvrScanning(false),
    m_imageCache(new XbmcImageCache(this))
{

    // Load debug stuff
    XDebug::addAllowedArea(XDAREA_GENERAL);
#ifdef QT5_BUILD
    QStringList args = QGuiApplication::arguments();
#else
    QStringList args = QApplication::arguments();
#endif
    qDebug() << "args are" << args;
    for(int i = 1; i < args.count(); ++i ) {
        if(args.at(i) == "-d") {
            if(args.count() > i) {
                QStringList debuglist = args.at(i + 1).split(',');
                foreach(const QString &debugString, debuglist) {
                    if(debugString == "connection") {
                        XDebug::addAllowedArea(XDAREA_CONNECTION);
                    } else if(debugString == "player") {
                        XDebug::addAllowedArea(XDAREA_PLAYER);
                    } else if(debugString == "library") {
                        XDebug::addAllowedArea(XDAREA_LIBRARY);
                    } else if(debugString == "files") {
                        XDebug::addAllowedArea(XDAREA_FILES);
                    } else if(debugString == "playlist") {
                        XDebug::addAllowedArea(XDAREA_PLAYLIST);
                    } else if(debugString == "networkdata") {
                        XDebug::addAllowedArea(XDAREA_NETWORKDATA);
                    } else if(debugString == "eventclient") {
                        XDebug::addAllowedArea(XDAREA_EVENTCLIENT);
//                    } else if(debugString == "") {
//                        XDebug::addAllowedArea(XDAREA_);
                    }
                }
            }
        }
    }

    qmlRegisterUncreatableType<Xbmc>("Xbmc", 1, 0, "Xbmc", "use context property xbmc");
    qmlRegisterType<AudioLibrary>();
    qmlRegisterType<VideoLibrary>();
    qmlRegisterType<LibraryItem>();
    qmlRegisterType<XbmcModelItem>();
    qmlRegisterType<XbmcModel>();
    qmlRegisterType<XbmcLibrary>();
    qmlRegisterType<Player>();
    qmlRegisterType<AudioPlayer>();
    qmlRegisterType<VideoPlayer>();
    qmlRegisterType<PicturePlayer>();
    qmlRegisterType<Playlist>();
    qmlRegisterType<PlaylistItem>();
    qmlRegisterType<AudioPlaylistItem>();
    qmlRegisterType<VideoPlaylistItem>();
    qmlRegisterType<Files>();
    qmlRegisterType<Shares>();
    qmlRegisterType<ChannelGroups>();
    qmlRegisterType<Channels>();
    qmlRegisterType<Keys>();
    qmlRegisterType<EventClient>();
    qmlRegisterType<XbmcFilterModel>("Xbmc", 1, 0, "XbmcFilterModel");

    // Hack: QML seems to have problems with enums exposed by a qmlRegisterUncreatableType
    // Because XbmcModel and Player are abstract, lets register one of their subclasses
    qmlRegisterType<Artists>("Xbmc", 1, 0, "XbmcModel");
    qmlRegisterType<AudioPlayer>("Xbmc", 1, 0, "Player");

    qmlRegisterType<XbmcHostModel>();
    qmlRegisterType<XbmcDiscovery>("Xbmc", 1, 0, "XbmcDiscovery");

    m_hosts = new XbmcHostModel(this);

    connect(XbmcConnection::notifier(), SIGNAL(connectionChanged()), SLOT(connectionChanged()));
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(parseAnnouncement(QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(authenticationRequired(QString,QString)), SIGNAL(authenticationRequired(QString, QString)));
    connect(XbmcConnection::notifier(), SIGNAL(downloadAdded(XbmcDownload*)), SLOT(slotDownloadAdded(XbmcDownload*)));

    m_audioPlayer = new AudioPlayer(this);
    m_videoPlayer = new VideoPlayer(this);
    m_picturePlayer = new PicturePlayer(this);
    m_activePlayer = 0;
    m_state = "undefined";
    m_picturePlayerActive = false;

    m_keys = new Keys(this);
    m_eventClient = new EventClient(this);

    m_volumeAnimation.setTargetObject(this);
    m_volumeAnimation.setPropertyName("volume");
    m_volumeAnimation.setDuration(500);

    m_imageCache->start(QThread::IdlePriority);
}

Xbmc::~Xbmc()
{
}

void Xbmc::init()
{
    m_eventClient->connectToHost(connectedHost());

    queryActivePlayers();

    QVariantMap params;
    QVariantList list;
    list.append("volume");
    params.insert("properties", list);
    XbmcConnection::sendCommand("Application.GetProperties", params, this, "volumeReceived");

    params.clear();
    list.clear();
    list.append("canshutdown");
    list.append("canhibernate");
    list.append("cansuspend");
    list.append("canreboot");
    params.insert("properties", list);
    XbmcConnection::sendCommand("System.GetProperties", params, this, "systemPropertiesReceived");

    params.clear();
    list.clear();
    list.append("available");
    list.append("recording");
    list.append("scanning");
    params.insert("properties", list);
    XbmcConnection::sendCommand("PVR.GetProperties", params, this, "pvrPropertiesReceived");
}

void Xbmc::slotDownloadAdded(XbmcDownload *download)
{
    connect(download, SIGNAL(finished(bool)), SLOT(downloadFinished(bool)));
    emit downloadAdded(download);
}

void Xbmc::downloadFinished(bool success)
{
    XbmcDownload *download = qobject_cast<XbmcDownload*>(sender());
    if(success) {
        emit displayNotification(tr("Finished downloading %1").arg(download->label()));
    } else {
        emit displayNotification(tr("Error downloading %1").arg(download->label()));
    }
}

bool Xbmc::connected()
{
    return XbmcConnection::connected();
}

XbmcHost *Xbmc::connectedHost()
{
    return XbmcConnection::connectedHost();
}

QString Xbmc::connectedHostName()
{
    if(XbmcConnection::connected()) {
        return XbmcConnection::connectedHost()->hostname();
    }
    return QString();
}

void Xbmc::setAuthCredentials(const QString &username, const QString &password)
{
    XbmcConnection::setAuthCredentials(username, password);
}

QString Xbmc::connectionError()
{
    return XbmcConnection::connectionError();
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

ChannelGroups *Xbmc::channelGroups()
{
    return new ChannelGroups();
}

AudioPlayer *Xbmc::audioPlayer()
{
    return m_audioPlayer;
}

VideoPlayer *Xbmc::videoPlayer()
{
    return m_videoPlayer;
}

PicturePlayer *Xbmc::picturePlayer()
{
    return m_picturePlayer;
}

XbmcHostModel *Xbmc::hostModel()
{
    return m_hosts;
}

Player *Xbmc::activePlayer()
{
//    qDebug() << "returning active player" << m_activePlayer;
    return m_activePlayer;
}

void Xbmc::parseAnnouncement(const QVariantMap &map)
{
    if(map.value("method").toString() == "Player.OnPlay") {
//        if(data.value("player").toMap().value("playerid").toInt() != m_activePlayer->playerId()) {
            queryActivePlayers();
//        }
    }
    else if(map.value("method").toString() == "Player.OnStop") {
        queryActivePlayers();
    } else if(map.value("method").toString() == "Application.OnVolumeChanged") {
        qDebug() << "volume changed";
        m_volume = map.value("params").toMap().value("data").toMap().value("volume").toInt();
        emit volumeChanged(m_volume);
    }
}

void Xbmc::activePlayersReceived(const QVariantMap &rsp)
{
    Player *activePlayer = 0;

    QVariant result = rsp.value("result");

    // {"id":8,"jsonrpc":"2.0","result":[{"playerid":0,"type":"audio"}]}
    xDebug(XDAREA_PLAYER) << "active player response:" << result;
    QVariantList activePlayers = result.toList();
    bool picturesActive = false;
    foreach(const QVariant &activePlayerMap, activePlayers) {
        if(activePlayerMap.toMap().value("type").toString() == "audio") {
            activePlayer = m_audioPlayer;
            m_state = "audio";
        } else if(activePlayerMap.toMap().value("type").toString() == "video") {
            activePlayer = m_videoPlayer;
            m_state = "video";
        }
        if(activePlayerMap.toMap().value("type").toString() == "picture"){
            picturesActive = true;
        }
    }

    if(activePlayer == 0) {
        if(m_state != "") {
            m_state = "";
            emit stateChanged();
        }

        if(m_activePlayer) {
            m_activePlayer->detach();
            m_activePlayer = 0;
            emit activePlayerChanged();
        }
    } else if(m_activePlayer != activePlayer) {
        if(m_activePlayer) {
            m_activePlayer->detach();
        }
        m_activePlayer = activePlayer;
        xDebug(XDAREA_PLAYER) << "active player changed!";
        emit stateChanged();
        emit activePlayerChanged();
        m_activePlayer->refresh();
    }

    if(m_picturePlayerActive != picturesActive) {
        m_picturePlayerActive = picturesActive;
        emit picturePlayerActiveChanged();
    }
}

void Xbmc::volumeReceived(const QVariantMap &rsp)
{
    m_volume = rsp.value("result").toMap().value("volume").toInt();
//    qDebug() << "Volume received" << m_volume;
    emit volumeChanged(m_volume);
}

void Xbmc::systemPropertiesReceived(const QVariantMap &rsp)
{
    qDebug() << "Got system properties:" << rsp.value("result").toMap();
    m_canShutdown = rsp.value("result").toMap().value("canshutdown").toBool();
    m_canReboot = rsp.value("result").toMap().value("canreboot").toBool();
    m_canHibernate = rsp.value("result").toMap().value("canhibernate").toBool();
    m_canSuspend = rsp.value("result").toMap().value("cansuspend").toBool();
    emit systemPropertiesChanged();
    qDebug() << m_canShutdown << m_canReboot << m_canHibernate << m_canSuspend;
}

void Xbmc::pvrPropertiesReceived(const QVariantMap &rsp)
{
    bool newValue = rsp.value("result").toMap().value("available").toBool();
    if(m_pvrAvailable != newValue) {
        m_pvrAvailable = newValue;
        emit pvrAvailableChanged();
    }
    newValue = rsp.value("result").toMap().value("recording").toBool();
    if(m_pvrRecording != newValue) {
        m_pvrRecording = newValue;
        emit pvrRecordingChanged();
    }
    newValue = rsp.value("result").toMap().value("scanning").toBool();
    if(m_pvrScanning != newValue) {
        m_pvrScanning = newValue;
        emit pvrScanningChanged();
    }
}

QString Xbmc::vfsPath()
{
    if(XbmcConnection::connectedHost()) {
        //xDebug(XDAREA_CONNECTION) << "returning vfs:" << "http://" + m_hostName + ':' + QString::number(m_port) + "/vfs/";
        return "http://" + XbmcConnection::connectedHost()->address() + ':' + QString::number(XbmcConnection::connectedHost()->port()) + "/vfs/";
    }
    return QString();
}

QString Xbmc::state()
{
    return m_state;
}

Keys *Xbmc::keys()
{
    return m_keys;
}

EventClient *Xbmc::eventClient()
{
    return m_eventClient;
}

bool Xbmc::connecting()
{
    return XbmcConnection::connecting();
}

void Xbmc::connectionChanged()
{
    if(connected()) {
        init();
    } else {
        m_eventClient->disconnectFromHost();
    }
    emit connectedChanged(connected());
}

void Xbmc::setVolume(int volume)
{
    if(volume < 0) {
        volume = 0;
    }
    if(volume > 100) {
        volume = 100;
    }

    if(XbmcConnection::connectedHost() && !XbmcConnection::connectedHost()->volumeUpCommand().isEmpty() && !XbmcConnection::connectedHost()->volumeUpCommand().isEmpty()) {
        QString cmd;
        QStringList args;
        if(volume < m_volume) {
            args = XbmcConnection::connectedHost()->volumeDownCommand().split(" ");
            cmd = args.takeFirst();
        } else {
            args = XbmcConnection::connectedHost()->volumeUpCommand().split(" ");
            cmd = args.takeFirst();
        }
        args << QString::number(volume);
        qDebug() << "executing command:" << cmd << args << QProcess::execute(cmd, args);

    } else {
        if(volume != m_volume) {
            QVariantMap map;
            map.insert("volume", volume);
            XbmcConnection::sendCommand("Application.SetVolume", map);

        }
    }
}

int Xbmc::volume()
{
    return m_volume;
}

void Xbmc::quit()
{
    XbmcConnection::sendCommand("Application.Quit");
}

void Xbmc::suspend()
{
    XbmcConnection::sendCommand("System.Suspend");
}

void Xbmc::hibernate()
{
    XbmcConnection::sendCommand("System.Hibernate");
}

void Xbmc::shutdown()
{
    XbmcConnection::sendCommand("System.Shutdown");
}

void Xbmc::reboot()
{
    XbmcConnection::sendCommand("System.Reboot");
}

void Xbmc::dimVolumeTo(int newVolume)
{
    m_volumeAnimation.setDirection(QAbstractAnimation::Forward);
    m_volumeAnimation.setStartValue(m_volume);
    m_volumeAnimation.setEndValue(newVolume);
    m_volumeAnimation.start();
}

void Xbmc::restoreVolume()
{
    m_volumeAnimation.setDirection(QAbstractAnimation::Backward);
    m_volumeAnimation.start();
}

void Xbmc::sendNotification(const QString &header, const QString &text)
{
    QVariantMap params;
    params.insert("title", header);
    params.insert("message", text);
    XbmcConnection::sendCommand("GUI.ShowNotification", params);
}

bool Xbmc::picturePlayerActive()
{
    return m_picturePlayerActive;
}

void Xbmc::queryActivePlayers()
{
    XbmcConnection::sendCommand("Player.GetActivePlayers", QVariantMap(), this, "activePlayersReceived");
}

void Xbmc::switchToWindow(Xbmc::GuiWindow window)
{
    QVariantMap params;

    switch(window) {
    case GuiWindowHome:
        params.insert("window", "home");
        break;
    case GuiWindowPictures:
        params.insert("window", "pictures");
        break;
    case GuiWindowMusic:
        params.insert("window", "music");
        break;
    case GuiWindowVideos:
        params.insert("window", "videos");
        break;
    case GuiWindowLiveTV:
        params.insert("window", "tv");
        break;
    }

    XbmcConnection::sendCommand("Gui.ActivateWindow", params);
}

bool Xbmc::canShutdown()
{
    return m_canShutdown;
}

bool Xbmc::canReboot()
{
    return m_canReboot;
}
bool Xbmc::canHibernate()
{
    return m_canHibernate;
}
bool Xbmc::canSuspend()
{
    return m_canSuspend;
}

bool Xbmc::pvrAvailable()
{
    return m_pvrAvailable;
}

bool Xbmc::pvrRecording()
{
    return m_pvrRecording;
}

bool Xbmc::pvrScanning()
{
    return m_pvrScanning;
}

XbmcImageCache *Xbmc::imageCache()
{
    return m_imageCache;
}
