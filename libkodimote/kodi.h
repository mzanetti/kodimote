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

#ifndef XBMC_H
#define XBMC_H

#include <QObject>
#include <QVariantMap>
#include <QPropertyAnimation>

class KodiModel;
class KodiHost;
class KodiHostModel;
class AudioLibrary;
class VideoLibrary;
class Shares;
class ChannelGroups;
class PvrMenu;
class Profiles;

class Player;
class AudioPlayer;
class VideoPlayer;
class PicturePlayer;

class Keys;
class EventClient;

class KodiDownload;

class KodiImageCache;

class Kodi : public QObject
{
    Q_OBJECT
    Q_ENUMS(GuiWindow)

    Q_PROPERTY(bool connecting READ connecting NOTIFY connectingChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QString connectionError READ connectionError NOTIFY connectedChanged)
    Q_PROPERTY(KodiHost* connectedHost READ connectedHost NOTIFY connectedChanged)
    Q_PROPERTY(QString connectedHostName READ connectedHostName NOTIFY connectedChanged)
    Q_PROPERTY(QString vfsPath READ vfsPath NOTIFY vfsPathChanged)
    Q_PROPERTY(Player* activePlayer READ activePlayer NOTIFY activePlayerChanged)
    Q_PROPERTY(bool picturePlayerActive READ picturePlayerActive NOTIFY picturePlayerActiveChanged)
    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)

    Q_PROPERTY(bool canShutdown READ canShutdown NOTIFY systemPropertiesChanged)
    Q_PROPERTY(bool canReboot READ canReboot NOTIFY systemPropertiesChanged)
    Q_PROPERTY(bool canHibernate READ canHibernate NOTIFY systemPropertiesChanged)
    Q_PROPERTY(bool canSuspend READ canSuspend NOTIFY systemPropertiesChanged)

    Q_PROPERTY(bool pvrAvailable READ pvrAvailable NOTIFY pvrAvailableChanged)
    Q_PROPERTY(bool pvrRecording READ pvrRecording NOTIFY pvrRecordingChanged)
    Q_PROPERTY(bool pvrScanning READ pvrScanning NOTIFY pvrScanningChanged)

public:
    enum GuiWindow {
        GuiWindowHome,
        GuiWindowPictures,
        GuiWindowMusic,
        GuiWindowVideos,
        GuiWindowLiveTV
    };

    static Kodi *instance();
    ~Kodi();

    Q_INVOKABLE AudioLibrary *audioLibrary();
    Q_INVOKABLE VideoLibrary *videoLibrary();

    Q_INVOKABLE Shares *shares(const QString &mediatype);
    Q_INVOKABLE PvrMenu *pvrMenu();
    Q_INVOKABLE Profiles *profiles();

    Q_INVOKABLE AudioPlayer *audioPlayer();
    Q_INVOKABLE VideoPlayer *videoPlayer();
    Q_INVOKABLE PicturePlayer *picturePlayer();
    Q_INVOKABLE Player *activePlayer();
    Q_INVOKABLE bool picturePlayerActive();

    Q_INVOKABLE Keys *keys();
    Q_INVOKABLE EventClient *eventClient();

    bool connecting();
    bool connected();
    KodiHost *connectedHost();
    QString connectedHostName();
    QString connectionError();
    Q_INVOKABLE KodiHostModel* hostModel();
    Q_INVOKABLE void setAuthCredentials(const QString &username, const QString &password);

    bool active() const;
    void setActive(bool active);

    QString state();

    QString vfsPath();

    Q_INVOKABLE void setVolume(int volume);
    int volume();

    Q_INVOKABLE void dimVolumeTo(int newVolume);
    Q_INVOKABLE void restoreVolume();
    Q_INVOKABLE void volumeUp();
    Q_INVOKABLE void volumeDown();

    Q_INVOKABLE void sendNotification(const QString &header, const QString &text);


    bool canShutdown();
    bool canReboot();
    bool canHibernate();
    bool canSuspend();

    bool pvrAvailable();
    bool pvrRecording();
    bool pvrScanning();

    KodiImageCache *imageCache();

    QString dataPath() const;
    void setDataPath(const QString &path);

public slots:
    void disconnectFromHost();
    void quit();
    void suspend();
    void hibernate();
    void shutdown();
    void reboot();
    void queryActivePlayers();
    void switchToWindow(GuiWindow window);

signals:
    void connectingChanged();
    void connectedChanged(bool connected);
    void activeChanged(bool connect);
    void authenticationRequired(const QString &hostname, const QString &address);
    void activePlayerChanged();
    void volumeChanged(int volume);
    void vfsPathChanged();
    void stateChanged();
    void portChanged();
    void picturePlayerActiveChanged();

    void systemPropertiesChanged();

    void downloadAdded(KodiDownload* download);

    void displayNotification(const QString &text);

    void pvrAvailableChanged();
    void pvrRecordingChanged();
    void pvrScanningChanged();

private slots:
    void parseAnnouncement(const QVariantMap &map);
    void connectionChanged();
    void init();
    void slotDownloadAdded(KodiDownload *download);
    void downloadFinished(bool success);
    void requestHwAddr();

    void activePlayersReceived(const QVariantMap &rsp);
    void volumeReceived(const QVariantMap &rsp);
    void systemPropertiesReceived(const QVariantMap &rsp);
    void pvrPropertiesReceived(const QVariantMap &rsp);
    void hwAddrReceived(const QVariantMap &rsp);

private:
    static Kodi *s_instance;
    explicit Kodi(QObject *parent = 0);

    void setActivePlayer(Player *player, bool picturePlayerActive);

    AudioPlayer *m_audioPlayer;
    VideoPlayer *m_videoPlayer;
    PicturePlayer *m_picturePlayer;
    Player *m_activePlayer;
    bool m_picturePlayerActive;

    Keys *m_keys;
    EventClient *m_eventClient;

    int m_volume;
    QString m_state;
    int m_hwAddrRequestCount;

    QString m_hostname;
    int m_port;
    QString m_username;
    QString m_password;
    KodiHostModel *m_hosts;
    Profiles *m_profiles;

    int m_originalVolume;
    int m_targetVolume;

    QPropertyAnimation m_volumeAnimation;

    bool m_canShutdown;
    bool m_canReboot;
    bool m_canHibernate;
    bool m_canSuspend;

    bool m_pvrAvailable;
    bool m_pvrRecording;
    bool m_pvrScanning;

    KodiImageCache *m_imageCache;
    QString m_dataPath;
};

#endif // XBMC_H
