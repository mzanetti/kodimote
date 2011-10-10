#include "settings.h"

#include <QSettings>

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    // Write defaults for hidden configs to file so that users have a chance to find them
    QSettings settings("xbmcremote");
    if(!settings.contains("VolumeUpCommand")) {
        settings.setValue("VolumeUpCommand", QString());
    }
    if(!settings.contains("VolumeDownCommand")) {
        settings.setValue("VolumeDownCommand", QString());
    }
}

bool Settings::themeInverted() const
{
    QSettings settings("xbmcremote");
    return settings.value("InvertTheme", false).toBool();
}

void Settings::setThemeInverted(bool inverted)
{
    QSettings settings("xbmcremote");
    settings.setValue("InvertTheme", inverted);
}

void Settings::setVolumeUpCommand(const QString &volumeUpCommand)
{
    QSettings settings("xbmcremote");
    settings.setValue("VolumeUpCommand", volumeUpCommand);
}

QString Settings::volumeUpCommand() const
{
    QSettings settings("xbmcremote");
    return settings.value("VolumeUpCommand", QString()).toString();
}

void Settings::setVolumeDownCommand(const QString &volumeDownCommand)
{
    QSettings settings("xbmcremote");
    settings.setValue("VolumeDownCommand", volumeDownCommand);
}

QString Settings::volumeDownCommand() const
{
    QSettings settings("xbmcremote");
    return settings.value("VolumeDownCommand", QString()).toString();
}

void Settings::setVolumeOnCall(int volume)
{
    QSettings settings("xbmcremote");
    settings.setValue("VolumeOnCall", volume);
}

int Settings::volumeOnCall() const
{
    QSettings settings("xbmcremote");
    return settings.value("VolumeOnCall", 10).toInt();
}

bool Settings::changeVolumeOnCall() const
{
    QSettings settings("xbmcremote");
    return settings.value("ChangeVolumeOnCall", true).toBool();
}

void Settings::setChangeVolumeOnCall(bool changeVolume)
{
    QSettings settings("xbmcremote");
    settings.setValue("ChangeVolumeOnCall", changeVolume);
}

bool Settings::pauseOnCall() const
{
    QSettings settings("xbmcremote");
    return settings.value("PauseOnCall", true).toBool();
}

void Settings::setPauseOnCall(bool pause)
{
    QSettings settings("xbmcremote");
    settings.setValue("PauseOnCall", pause);
}

