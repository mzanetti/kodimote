#ifndef SETTINGS_H
#define SETTINGS_H

#include "xbmc/xbmchostmodel.h"

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool themeInverted READ themeInverted WRITE setThemeInverted NOTIFY themeInvertedChanged)
    Q_PROPERTY(bool useThumbnails READ useThumbnails WRITE setUseThumbnails NOTIFY useThumbnailsChanged)
    Q_PROPERTY(bool changeVolumeOnCall READ changeVolumeOnCall WRITE setChangeVolumeOnCall NOTIFY changeVolumeOnCallChanged)
    Q_PROPERTY(int volumeOnCall READ volumeOnCall WRITE setVolumeOnCall NOTIFY volumeOnCallChanged)
    Q_PROPERTY(bool pauseVideoOnCall READ pauseVideoOnCall WRITE setPauseVideoOnCall NOTIFY pauseVideoOnCallChanged)
    Q_PROPERTY(bool pauseMusicOnCall READ pauseMusicOnCall WRITE setPauseMusicOnCall NOTIFY pauseMusicOnCallChanged)
    Q_PROPERTY(bool keepDisplayLit READ keepDisplayLit WRITE setKeepDisplayLit NOTIFY keepDisplayLitChanged)
    Q_PROPERTY(bool showCallNotifications READ showCallNotifications WRITE setShowCallNotifications NOTIFY showCallNotificationsChanged)

public:
    explicit Settings(QObject *parent = 0);

    void setThemeInverted(bool inverted);
    bool themeInverted() const;

    void setUseThumbnails(bool useThumbnails);
    bool useThumbnails() const;

    void setChangeVolumeOnCall(bool changeVolume);
    bool changeVolumeOnCall() const;

    void setVolumeOnCall(int volume);
    int volumeOnCall() const;

    void setShowCallNotifications(bool show);
    bool showCallNotifications();

    void setPauseVideoOnCall(bool pauseOnCall);
    bool pauseVideoOnCall() const;

    void setPauseMusicOnCall(bool pauseOnCall);
    bool pauseMusicOnCall() const;

    QList<XbmcHost> hostList() const;
    void addHost(const XbmcHost &host);
    void removeHost(const XbmcHost &host);

    XbmcHost lastHost() const;
    void setLastHost(const XbmcHost &host);

    bool keepDisplayLit() const;
    void setKeepDisplayLit(bool keepLit);

signals:
    void themeInvertedChanged();
    void useThumbnailsChanged();
    void volumeUpCommandChanged();
    void volumeDownCommandChanged();
    void changeVolumeOnCallChanged();
    void volumeOnCallChanged();
    void pauseVideoOnCallChanged();
    void pauseMusicOnCallChanged();
    void keepDisplayLitChanged();
    void showCallNotificationsChanged();
};

#endif // SETTINGS_H
