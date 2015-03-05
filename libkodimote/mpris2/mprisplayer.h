#ifndef MPRISPLAYER_H
#define MPRISPLAYER_H

#include <QtDBus/QtDBus>

#include "protocolhandlers/protocolmanager.h"

class Player;
class LibraryItem;

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
    Q_PROPERTY(double Volume READ volume WRITE setVolume)
    Q_PROPERTY(bool Shuffle READ shuffle WRITE setShuffle)
    Q_PROPERTY(QString LoopStatus READ loopStatus WRITE setLoopStatus)
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
public:
    explicit MprisPlayer(ProtocolManager *protocols, QObject *parent = 0);

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

    double volume() const;
    void setVolume(double volume);

    bool shuffle() const;
    void setShuffle(bool shuffle);

    QString loopStatus() const;
    void setLoopStatus(QString loopStatus);

signals:
    void Seeked(qint64);

public slots:
    void Next();
    void Previous();
    void Pause();
    void PlayPause();
    void Play();
    void Stop();
    void Seek(qint64 offset);
    void SetPosition(QDBusObjectPath path, qint64 position);
    void OpenUri(QString uri);

private:
    Player *m_player;
    ProtocolManager *m_protocols;

    QString buildPath(LibraryItem *item) const;

    void sendPropertyChanged(const QString &property);
    void sendPropertiesChanged(const QStringList &properties);

private slots:
    void activePlayerChanged();
    void volumeChanged();
    void currentItemChanged();
    void stateChanged();
    void timeChanged();
    void speedChanged();
    void shuffleChanged();
    void repeatChanged();
    void playlistChanged();
    void thumbnailChanged();
};

#endif // MPRISPLAYER_H
