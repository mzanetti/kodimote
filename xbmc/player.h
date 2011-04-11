#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

namespace Xbmc
{

class Playlist;

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(PlayerType type READ type)
    Q_PROPERTY(Playlist playlist READ playlist)

public:
    enum PlayerType {
        PlayerTypeAudio,
        PlayerTypeVideo,
        PlayerTypePictures
    };

    enum State {
        StatePlaying,
        StatePaused,
        StatePartyMode
    };

    explicit Player(PlayerType type, QObject *parent = 0);

    State state();

    PlayerType type();

    virtual QString namespaceString() const = 0;

    virtual Playlist* playlist() const = 0;

signals:

public slots:
    void playPause();
    void stop();
    void skipPrevious();
    void skipNext();
    void seekBackward();
    void seekForward();

private:
    PlayerType m_type;
};

}

#endif // PLAYER_H
