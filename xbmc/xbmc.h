#ifndef XBMC_H
#define XBMC_H

#include <QObject>
#include <QVariantMap>

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

signals:
    void connectedChanged();
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

};

#endif // XBMC_H
