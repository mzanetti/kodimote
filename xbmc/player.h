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

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QVariantMap>

namespace Xbmc
{

class Playlist;

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(PlayerType type READ type)
    Q_PROPERTY(Playlist playlist READ playlist)
    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)

public:
    enum PlayerType {
        PlayerTypeAudio,
        PlayerTypeVideo,
        PlayerTypePictures
    };

    explicit Player(PlayerType type, QObject *parent = 0);

    QString state();
    int speed();

    PlayerType type();

    virtual QString namespaceString() const = 0;

    virtual Playlist* playlist() const = 0;

signals:
    void stateChanged();
    void speedChanged();

public slots:
    void playPause();
    void stop();
    void skipPrevious();
    void skipNext();
    void seekBackward();
    void seekForward();

private slots:
    void getState();
    void receivedAnnouncement(const QVariantMap& map);
    void responseReceived(int, const QVariantMap &rsp);

private:
    enum Request {
       RequestState
    };
    QMap<int, Request> m_requestMap;

    PlayerType m_type;
    QString m_state;
    int m_speed;
};

}

#endif // PLAYER_H
