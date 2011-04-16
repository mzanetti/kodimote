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
