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
    Q_PROPERTY(QVariantMap Metadata READ metadata)
    Q_PROPERTY(QString PlaybackStatus READ playbackStatus)
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
public:
    explicit MprisPlayer(QObject *parent = 0);

    bool canControl() const;
    bool canGoNext() const;
    bool canGoPrevious() const;
    bool canPause() const;
    bool canPlay() const;
    bool canSeek() const;
    QVariantMap metadata() const;
    QString playbackStatus() const;

public slots:
    void Next();
    void Previous();
    void Pause();
    void PlayPause();
    void Play();
    void Stop();
    //void Seek(int64_t offset);
    //void SetPosition(QDBusObjectPath path, int64_t position);
    //void OpenUri(QString uri);

private:
    Player *activePlayer() const;
};

#endif // MPRISPLAYER_H
