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
#include <QTimer>

class Playlist;

class Player : public QObject
{
    Q_OBJECT
    Q_ENUMS(PlayerType)
    Q_ENUMS(Repeat)

    Q_PROPERTY(PlayerType type READ type NOTIFY typeChanged)
//    Q_PROPERTY(Playlist playlist READ playlist NOTIFY playlistChanged)
    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(double percentage READ percentage NOTIFY percentageChanged)
    Q_PROPERTY(QString time READ time NOTIFY timeChanged)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(Repeat repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)

public:
    enum PlayerType {
        PlayerTypeAudio,
        PlayerTypeVideo,
        PlayerTypePictures
    };
    enum Repeat {
        RepeatNone,
        RepeatOne,
        RepeatAll
    };

    Player() {};
    explicit Player(PlayerType type, QObject *parent = 0);

    QString state() const;
    int speed() const;
    double percentage() const;
    QString time() const;

    void refresh();

    PlayerType type() const;

    virtual int playerId() const = 0;

    Q_INVOKABLE virtual Playlist* playlist() const = 0;

    /// play the given item from the playlist
    Q_INVOKABLE void playItem(int index);

    bool shuffle() const;
    void setShuffle(bool shuffle);

    Repeat repeat() const;
    void setRepeat(Repeat repeat);

signals:
    void stateChanged();
    void speedChanged();
    void percentageChanged();
    void typeChanged();
    void timeChanged();
    void shuffleChanged();
    void repeatChanged();

public slots:
    void playPause();
    void stop();
    void skipPrevious();
    void skipNext();
    void seekBackward();
    void seekForward();

private slots:
    void getSpeed();
    void getPercentage();
    void getPosition();
    void receivedAnnouncement(const QVariantMap& map);
    void responseReceived(int, const QVariantMap &rsp);
    void setPercentage();
    void getRepeatShuffle();

protected:
    enum Request {
        RequestSpeed,
        RequestPercentage,
        RequestPosition,
        RequestRepeatShuffle
    };
    QMap<int, Request> m_requestMap;

    PlayerType m_type;
    QString m_state;
    int m_speed;
    double m_percentage;
    QTimer m_percentageTimer;
    int m_currentItem;

    bool m_shuffle;
    Repeat m_repeat;
};

#endif // PLAYER_H
