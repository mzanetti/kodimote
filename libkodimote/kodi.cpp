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

#include "kodi.h"
#include "kodihostmodel.h"
#include "kodifiltermodel.h"
#include "kodiconnection.h"
#include "kodebug.h"
#include "kodidiscovery.h"
#include "kodidownload.h"

#include "audiolibrary.h"
#include "artists.h"

#include "videolibrary.h"

#include "files.h"
#include "shares.h"
#include "channelgroups.h"
#include "channels.h"
#include "channelbroadcasts.h"
#include "pvrmenu.h"
#include "profiles.h"

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

#include "settings.h"

#include "imagecache.h"


#ifdef QT5_BUILD
#include <QtQuick>
#else
#include <QtDeclarative>
#endif

Kodi *Kodi::s_instance = 0;

Kodi *Kodi::instance()
{
    if(!s_instance) {
        s_instance = new Kodi();
    }
    return s_instance;
}

Kodi::Kodi(QObject *parent) :
    QObject(parent),
    m_volume(0),
    m_canShutdown(false),
    m_canReboot(false),
    m_canHibernate(false),
    m_canSuspend(false),
    m_pvrAvailable(false),
    m_pvrRecording(false),
    m_pvrScanning(false),
    m_imageCache(new KodiImageCache(this)),
    m_dataPath(QDir::home().absolutePath() + "/.kodimote/")
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
                    } else if(debugString == "discovery") {
                        XDebug::addAllowedArea(XDAREA_DISCOVERY);
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

#ifdef SAILFISH
    // @uri harbour.kodimote
    const char *qmlUri = "harbour.kodimote";
#else
    // @uri Kodi
    const char *qmlUri = "Kodi";
#endif

    qmlRegisterUncreatableType<Kodi>(qmlUri, 1, 0, "Kodi", "use context property kodi");
    qmlRegisterType<AudioLibrary>();
    qmlRegisterType<VideoLibrary>();
    qmlRegisterType<LibraryItem>();
    qmlRegisterType<KodiModelItem>();
    qmlRegisterType<KodiModel>();
    qmlRegisterType<KodiLibrary>();
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
    qmlRegisterType<ChannelBroadcasts>();
    qmlRegisterType<PvrMenu>();
    qmlRegisterType<Profiles>();
    qmlRegisterType<Keys>();
    qmlRegisterType<EventClient>();
    qmlRegisterType<KodiFilterModel>(qmlUri, 1, 0, "KodiFilterModel");

    // Hack: QML seems to have problems with enums exposed by a qmlRegisterUncreatableType
    // Because KodiModel and Player are abstract, lets register one of their subclasses
    qmlRegisterType<Artists>(qmlUri, 1, 0, "KodiModel");
    qmlRegisterType<AudioPlayer>(qmlUri, 1, 0, "Player");

    qmlRegisterType<KodiHostModel>();
    qmlRegisterType<KodiHost>(qmlUri, 1, 0, "KodiHost");
    qmlRegisterType<KodiDiscovery>(qmlUri, 1, 0, "KodiDiscovery");

    qmlRegisterUncreatableType<Settings>(qmlUri, 1, 0, "Settings", "Cannot create multiple settings. Use the \"settings\" context property intead");

    m_hosts = new KodiHostModel(this);

    connect(KodiConnection::notifier(), SIGNAL(connectionChanged()), SLOT(connectionChanged()));
    connect(KodiConnection::notifier(), SIGNAL(connectionChanged()), SIGNAL(connectingChanged()));
    connect(KodiConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(parseAnnouncement(QVariantMap)));
    connect(KodiConnection::notifier(), SIGNAL(authenticationRequired(QString,QString)), SIGNAL(authenticationRequired(QString, QString)));
    connect(KodiConnection::notifier(), SIGNAL(downloadAdded(KodiDownload*)), SLOT(slotDownloadAdded(KodiDownload*)));

    m_audioPlayer = new AudioPlayer(this);
    m_videoPlayer = new VideoPlayer(this);
    m_picturePlayer = new PicturePlayer(this);
    m_activePlayer = 0;
    m_state = "undefined";
    m_picturePlayerActive = false;

    m_keys = new Keys(this);
    m_eventClient = new EventClient(this);
    m_profiles = new Profiles(this);

    m_volumeAnimation.setTargetObject(this);
    m_volumeAnimation.setPropertyName("volume");
    m_volumeAnimation.setDuration(500);
}

Kodi::~Kodi()
{
}

void Kodi::init()
{
    m_eventClient->connectToHost(connectedHost());

    queryActivePlayers();

    QVariantMap params;
    QVariantList list;
    list.append("volume");
    params.insert("properties", list);
    KodiConnection::sendCommand("Application.GetProperties", params, this, "volumeReceived");

    params.clear();
    list.clear();
    list.append("canshutdown");
    list.append("canhibernate");
    list.append("cansuspend");
    list.append("canreboot");
    params.insert("properties", list);
    KodiConnection::sendCommand("System.GetProperties", params, this, "systemPropertiesReceived");

    params.clear();
    list.clear();
    list.append("available");
    list.append("recording");
    list.append("scanning");
    params.insert("properties", list);
    KodiConnection::sendCommand("PVR.GetProperties", params, this, "pvrPropertiesReceived");

    if (KodiConnection::connectedHost()->hwAddr().isEmpty()) {
        m_hwAddrRequestCount = 0;
        requestHwAddr();
    }

    m_profiles->refresh();
}

void Kodi::slotDownloadAdded(KodiDownload *download)
{
    connect(download, SIGNAL(finished(bool)), SLOT(downloadFinished(bool)));
    emit downloadAdded(download);
}

void Kodi::downloadFinished(bool success)
{
    KodiDownload *download = qobject_cast<KodiDownload*>(sender());
    if(success) {
        emit displayNotification(tr("Finished downloading %1").arg(download->label()));
    } else {
        emit displayNotification(tr("Error downloading %1").arg(download->label()));
    }
}

bool Kodi::connected()
{
    return KodiConnection::connected();
}

KodiHost *Kodi::connectedHost()
{
    return KodiConnection::connectedHost();
}

QString Kodi::connectedHostName()
{
    if(KodiConnection::connected()) {
        return KodiConnection::connectedHost()->hostname();
    }
    return QString();
}

void Kodi::setAuthCredentials(const QString &username, const QString &password)
{
    KodiConnection::setAuthCredentials(username, password);
}

bool Kodi::active() const
{
    return KodiConnection::active();
}

void Kodi::setActive(bool active)
{
    KodiConnection::setActive(active);
    emit activeChanged(active);
}

QString Kodi::connectionError()
{
    return KodiConnection::connectionError();
}

AudioLibrary *Kodi::audioLibrary()
{
    return new AudioLibrary();
}

VideoLibrary *Kodi::videoLibrary()
{
    return new VideoLibrary();
}

Shares *Kodi::shares(const QString &mediatype)
{
    return new Shares(mediatype);
}

PvrMenu *Kodi::pvrMenu()
{
    return new PvrMenu();
}

Profiles *Kodi::profiles()
{
    return m_profiles;
}

AudioPlayer *Kodi::audioPlayer()
{
    return m_audioPlayer;
}

VideoPlayer *Kodi::videoPlayer()
{
    return m_videoPlayer;
}

PicturePlayer *Kodi::picturePlayer()
{
    return m_picturePlayer;
}

KodiHostModel *Kodi::hostModel()
{
    return m_hosts;
}

Player *Kodi::activePlayer()
{
//    qDebug() << "returning active player" << m_activePlayer;
    return m_activePlayer;
}

void Kodi::parseAnnouncement(const QVariantMap &map)
{
    if(map.value("method").toString() == "Player.OnPlay") {
//        if(data.value("player").toMap().value("playerid").toInt() != m_activePlayer->playerId()) {
            queryActivePlayers();
//        }
    }
    else if(map.value("method").toString() == "Player.OnStop") {
        QTimer::singleShot(500, this, SLOT(queryActivePlayers()));
    } else if(map.value("method").toString() == "Application.OnVolumeChanged") {
        qDebug() << "volume changed";
        m_volume = map.value("params").toMap().value("data").toMap().value("volume").toInt();
        emit volumeChanged(m_volume);
    }

    // Workaround: Kodi does not send activeplayerschanged when
    // the pictureplayer starts. We get only a Clear notification
    // for the picture playlist. Directly after onClear, the pictureplayer
    // isn't still active. Lets wait a sec...
    //
    // http://trac.kodi.org/ticket/14009

    if (map.value("method").toString() == "Playlist.OnClear") {
        if (map.value("params").toMap().value("data").toMap().value("playlistid").toInt() == m_picturePlayer->playlist()->playlistId()) {
            QTimer::singleShot(1000, this, SLOT(queryActivePlayers()));
        }
    }

}

void Kodi::activePlayersReceived(const QVariantMap &rsp)
{
    Player *activePlayer = 0;

    QVariant result = rsp.value("result");

    // {"id":8,"jsonrpc":"2.0","result":[{"playerid":0,"type":"audio"}]}
    koDebug(XDAREA_PLAYER) << "active player response:" << result;
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

    setActivePlayer(activePlayer, picturesActive);
}

void Kodi::volumeReceived(const QVariantMap &rsp)
{
    m_volume = rsp.value("result").toMap().value("volume").toInt();
//    qDebug() << "Volume received" << m_volume;
    emit volumeChanged(m_volume);
}

void Kodi::systemPropertiesReceived(const QVariantMap &rsp)
{
    qDebug() << "Got system properties:" << rsp.value("result").toMap();
    m_canShutdown = rsp.value("result").toMap().value("canshutdown").toBool();
    m_canReboot = rsp.value("result").toMap().value("canreboot").toBool();
    m_canHibernate = rsp.value("result").toMap().value("canhibernate").toBool();
    m_canSuspend = rsp.value("result").toMap().value("cansuspend").toBool();
    emit systemPropertiesChanged();
    qDebug() << m_canShutdown << m_canReboot << m_canHibernate << m_canSuspend;
}

void Kodi::pvrPropertiesReceived(const QVariantMap &rsp)
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

QString Kodi::vfsPath()
{
    if(KodiConnection::connectedHost()) {
        //koDebug(XDAREA_CONNECTION) << "returning vfs:" << "http://" + m_hostName + ':' + QString::number(m_port) + "/vfs/";
        return "http://" + KodiConnection::connectedHost()->address() + ':' + QString::number(KodiConnection::connectedHost()->port()) + "/vfs/";
    }
    return QString();
}

QString Kodi::state()
{
    return m_state;
}

Keys *Kodi::keys()
{
    return m_keys;
}

EventClient *Kodi::eventClient()
{
    return m_eventClient;
}

bool Kodi::connecting()
{
    return KodiConnection::connecting();
}

void Kodi::connectionChanged()
{
    if(connected()) {
        init();
    } else {
        m_eventClient->disconnectFromHost();
        setActivePlayer(0, false);
    }
    emit connectedChanged(connected());
}

void Kodi::setVolume(int volume)
{
    if(volume < 0) {
        volume = 0;
    }
    if(volume > 100) {
        volume = 100;
    }

    if(KodiConnection::connectedHost() && !KodiConnection::connectedHost()->volumeUpCommand().isEmpty() && !KodiConnection::connectedHost()->volumeUpCommand().isEmpty()) {
        QString cmd;
        QStringList args;
        if(volume < m_volume) {
            args = KodiConnection::connectedHost()->volumeDownCommand().split(" ");
            cmd = args.takeFirst();
        } else {
            args = KodiConnection::connectedHost()->volumeUpCommand().split(" ");
            cmd = args.takeFirst();
        }
        args << QString::number(volume);
        qDebug() << "executing command:" << cmd << args << QProcess::execute(cmd, args);

    } else {
        if(volume != m_volume) {
            QVariantMap map;
            map.insert("volume", volume);
            KodiConnection::sendCommand("Application.SetVolume", map);

        }
    }
}

int Kodi::volume()
{
    return m_volume;
}

void Kodi::quit()
{
    KodiConnection::sendCommand("Application.Quit");
}

void Kodi::suspend()
{
    KodiConnection::sendCommand("System.Suspend");
}

void Kodi::hibernate()
{
    KodiConnection::sendCommand("System.Hibernate");
}

void Kodi::shutdown()
{
    KodiConnection::sendCommand("System.Shutdown");
}

void Kodi::reboot()
{
    KodiConnection::sendCommand("System.Reboot");
}

void Kodi::dimVolumeTo(int newVolume)
{
    m_volumeAnimation.setDirection(QAbstractAnimation::Forward);
    m_volumeAnimation.setStartValue(m_volume);
    m_volumeAnimation.setEndValue(newVolume);
    m_volumeAnimation.start();
}

void Kodi::restoreVolume()
{
    m_volumeAnimation.setDirection(QAbstractAnimation::Backward);
    m_volumeAnimation.start();
}

void Kodi::volumeUp()
{
    KodiHost *host = KodiConnection::connectedHost();
    if (host) {
        KodiHost::VolumeControlType type = host->volumeControlType();
        if (type == KodiHost::VolumeControlTypeRelative) {
            QVariantMap map;
            map.insert("volume", "increment");
            KodiConnection::sendCommand("Application.SetVolume", map);
        } else {
            int stepping = host->volumeStepping();
            this->setVolume(m_volume + stepping);
        }
    }
}

void Kodi::volumeDown()
{
    KodiHost *host = KodiConnection::connectedHost();
    if (host) {
        KodiHost::VolumeControlType type = host->volumeControlType();
        if (type == KodiHost::VolumeControlTypeRelative) {
            QVariantMap map;
            map.insert("volume", "decrement");
            KodiConnection::sendCommand("Application.SetVolume", map);
        } else {
            int stepping = host->volumeStepping();
            this->setVolume(m_volume - stepping);
        }
    }
}

void Kodi::sendNotification(const QString &header, const QString &text)
{
    QVariantMap params;
    params.insert("title", header);
    params.insert("message", text);
    KodiConnection::sendCommand("GUI.ShowNotification", params);
}

bool Kodi::picturePlayerActive()
{
    return m_picturePlayerActive;
}

void Kodi::queryActivePlayers()
{
    KodiConnection::sendCommand("Player.GetActivePlayers", QVariantMap(), this, "activePlayersReceived");
}

void Kodi::switchToWindow(Kodi::GuiWindow window)
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

    KodiConnection::sendCommand("Gui.ActivateWindow", params);
}

bool Kodi::canShutdown()
{
    return m_canShutdown;
}

bool Kodi::canReboot()
{
    return m_canReboot;
}
bool Kodi::canHibernate()
{
    return m_canHibernate;
}
bool Kodi::canSuspend()
{
    return m_canSuspend;
}

bool Kodi::pvrAvailable()
{
    return m_pvrAvailable;
}

bool Kodi::pvrRecording()
{
    return m_pvrRecording;
}

bool Kodi::pvrScanning()
{
    return m_pvrScanning;
}

KodiImageCache *Kodi::imageCache()
{
    return m_imageCache;
}

QString Kodi::dataPath() const
{
    return m_dataPath;
}

void Kodi::setDataPath(const QString &path)
{
    m_dataPath = path;
}

void Kodi::disconnectFromHost()
{
    KodiConnection::disconnectFromHost();
}

void Kodi::requestHwAddr()
{
    m_hwAddrRequestCount++;
    QVariantMap params;
    QVariantList labels;
    labels.append("Network.MacAddress");
    params.insert("labels", labels);
    KodiConnection::sendParallelCommand("XBMC.GetInfoLabels", params, this, "hwAddrReceived");
}

void Kodi::hwAddrReceived(const QVariantMap &rsp)
{
    QString hwAddr = rsp.value("result").toMap().value("Network.MacAddress").toString();

    if (hwAddr.isEmpty()) {
        return;
    }

    //hwAddr can be a localized 'Busy' message, so we need to check if it's actually a hw address
    if (QRegExp("^([0-9A-F]{2}[:]){5}([0-9A-F]{2})$").exactMatch(hwAddr)) {
        KodiConnection::connectedHost()->setHwAddr(hwAddr.toLower());
    } else if(m_hwAddrRequestCount < 5) {
        QTimer::singleShot(100, this, SLOT(requestHwAddr()));
    }
}

void Kodi::setActivePlayer(Player *player, bool picturePlayerActive)
{
    if(player == 0) {
        if(m_state != "") {
            m_state = "";
            emit stateChanged();
        }

        if(m_activePlayer) {
            m_activePlayer->detach();
            m_activePlayer = 0;
            emit activePlayerChanged();
        }
    } else if(m_activePlayer != player) {
        if(m_activePlayer) {
            m_activePlayer->detach();
        }
        m_activePlayer = player;
        koDebug(XDAREA_PLAYER) << "active player changed!";
        emit stateChanged();
        emit activePlayerChanged();
        m_activePlayer->refresh();
    }

    if(m_picturePlayerActive != picturePlayerActive) {
        m_picturePlayerActive = picturePlayerActive;
        emit picturePlayerActiveChanged();
    }
}
