#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool themeInverted READ themeInverted WRITE setThemeInverted)
    Q_PROPERTY(bool changeVolumeOnCall READ changeVolumeOnCall WRITE setChangeVolumeOnCall)
    Q_PROPERTY(int volumeOnCall READ volumeOnCall WRITE setVolumeOnCall)
    Q_PROPERTY(bool pauseOnCall READ pauseOnCall WRITE setPauseOnCall)

public:
    explicit Settings(QObject *parent = 0);

    void setThemeInverted(bool inverted);
    bool themeInverted() const;

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

};

#endif // SETTINGS_H
