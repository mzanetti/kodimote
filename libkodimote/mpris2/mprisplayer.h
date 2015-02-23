#ifndef MPRISPLAYER_H
#define MPRISPLAYER_H

#include <QtDBus/QtDBus>

class Player;

class MprisPlayer : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_PROPERTY(bool CanControl READ canControl)
    Q_PROPERTY(bool CanGoNext READ canGoNext)
    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious)
    Q_PROPERTY(bool CanPause READ canPause)
    Q_PROPERTY(bool CanPlay READ canPlay)
    Q_PROPERTY(bool CanSeek READ canSeek)
    Q_PROPERTY(double MaximumRate READ maximumRate)
    Q_PROPERTY(double MinimumRate READ minimumRate)
    Q_PROPERTY(QVariantMap Metadata READ metadata)
    Q_PROPERTY(QString PlaybackStatus READ playbackStatus)
    Q_PROPERTY(qint64 Position READ position)
    Q_PROPERTY(double Rate READ rate)
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
public:
    explicit MprisPlayer(QObject *parent = 0);

    bool canControl() const;
    bool canGoNext() const;
    bool canGoPrevious() const;
    bool canPause() const;
    bool canPlay() const;
    bool canSeek() const;
    double maximumRate() const;
    double minimumRate() const;
    QVariantMap metadata() const;
    QString playbackStatus() const;
    qint64 position() const;
    double rate() const;

public slots:
    void Next();
    void Previous();
    void Pause();
    void PlayPause();
    void Play();
    void Stop();
    //void Seek(qint64 offset);
    //void SetPosition(QDBusObjectPath path, qint64 position);
    //void OpenUri(QString uri);

private:
    Player *m_player;

private slots:
    void activePlayerChanged();
    void currentItemChanged();
    void stateChanged();
    void timeChanged();
    void playlistChanged();
};

#endif // MPRISPLAYER_H
