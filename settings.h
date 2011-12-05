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
    Q_PROPERTY(bool pauseOnCall READ pauseOnCall WRITE setPauseOnCall NOTIFY pauseOnCallChanged)

public:
    explicit Settings(QObject *parent = 0);

    void setThemeInverted(bool inverted);
    bool themeInverted() const;

    void setUseThumbnails(bool useThumbnails);
    bool useThumbnails() const;

    void setVolumeUpCommand(const QString &volumeUpCommand);
    QString volumeUpCommand() const;

    void setVolumeDownCommand(const QString &volumeDownCommand);
    QString volumeDownCommand() const;

    void setChangeVolumeOnCall(bool changeVolume);
    bool changeVolumeOnCall() const;

    void setVolumeOnCall(int volume);
    int volumeOnCall() const;

    void setPauseOnCall(bool pauseOnCall);
    bool pauseOnCall() const;

    QList<XbmcHost> hostList() const;
    void addHost(const XbmcHost &host);
    void removeHost(const XbmcHost &host);

    XbmcHost lastHost() const;
    void setLastHost(const XbmcHost &host);

signals:
    void themeInvertedChanged();
    void useThumbnailsChanged();
    void volumeUpCommandChanged();
    void volumeDownCommandChanged();
    void changeVolumeOnCallChanged();
    void volumeOnCallChanged();
    void pauseOnCallChanged();
};

#endif // SETTINGS_H
