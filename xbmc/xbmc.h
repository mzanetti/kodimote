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

#ifndef XBMC_H
#define XBMC_H

#include <QObject>
#include <QVariantMap>
#include <QPropertyAnimation>

class XbmcModel;
class AudioLibrary;
class VideoLibrary;

class Player;
class AudioPlayer;
class VideoPlayer;
class Shares;

class Keys;

class Xbmc : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString connectionError READ connectionError NOTIFY connectedChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString vfsPath READ vfsPath NOTIFY vfsPathChanged)
    Q_PROPERTY(Player* activePlayer READ activePlayer NOTIFY activePlayerChanged)
    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)

public:
    static Xbmc *instance();
    ~Xbmc();

    Q_INVOKABLE AudioLibrary *audioLibrary();
    Q_INVOKABLE VideoLibrary *videoLibrary();

    Q_INVOKABLE Shares *shares(const QString &mediatype);

    Q_INVOKABLE AudioPlayer *audioPlayer();
    Q_INVOKABLE VideoPlayer *videoPlayer();
    Q_INVOKABLE Player *activePlayer();

    Q_INVOKABLE Keys *keys();

    bool connected();
    QString connectionError();
    Q_INVOKABLE void connectToHost();

    QString hostname();
    void setHostname(const QString &hostname);
    int port();
    void setPort(int port);

    QString state();

    QString vfsPath();

    Q_INVOKABLE void setVolume(int volume);
    int volume();

    Q_INVOKABLE void dimVolumeTo(int newVolume);
    Q_INVOKABLE void restoreVolume();

public slots:
    void quit();

signals:
    void connectedChanged(bool connected);
    void activePlayerChanged();
    void volumeChanged(int volume);
    void vfsPathChanged();
    void stateChanged();
    void hostnameChanged();
    void portChanged();

private slots:
    void parseAnnouncement(const QVariantMap &map);
    void responseReceived(int id, const QVariantMap &rsp);
    void connectionChanged();
    void init();

private:
    static Xbmc *s_instance;
    explicit Xbmc(QObject *parent = 0);

    enum Request {
        RequestActivePlayer,
        RequestVolume
    };
    QMap<int, Request> m_requestMap;

    AudioPlayer *m_audioPlayer;
    VideoPlayer *m_videoPlayer;
    Player *m_activePlayer;

    Keys *m_keys;

    int m_volume;
    QString m_state;

    QString m_hostname;
    int m_port;

    int m_originalVolume;
    int m_targetVolume;

    QPropertyAnimation m_volumeAnimation;

};

#endif // XBMC_H
