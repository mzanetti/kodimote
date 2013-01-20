#include "settings.h"

#include <QSettings>
#include <QStringList>

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
    return settings.value("InvertTheme", true).toBool();
}

void Settings::setThemeInverted(bool inverted)
{
    QSettings settings("xbmcremote");
    settings.setValue("InvertTheme", inverted);
    emit themeInvertedChanged();
}

bool Settings::useThumbnails() const
{
    QSettings settings("xbmcremote");
    return settings.value("UseThumbnails", true).toBool();
}

void Settings::setIgnoreArticle(bool ignoreArticle)
{
    QSettings settings("xbmcremote");
    settings.setValue("IgnoreArticle", ignoreArticle);
    emit ignoreArticleChanged();
}

bool Settings::ignoreArticle() const
{
    QSettings settings("xbmcremote");
    return settings.value("IgnoreArticle", true).toBool();
}

void Settings::setUseThumbnails(bool useThumbnails)
{
    QSettings settings("xbmcremote");
    settings.setValue("UseThumbnails", useThumbnails);
    emit useThumbnailsChanged();
}


void Settings::setVolumeOnCall(int volume)
{
    QSettings settings("xbmcremote");
    settings.setValue("VolumeOnCall", volume);
    emit volumeOnCallChanged();
}

int Settings::volumeOnCall() const
{
    QSettings settings("xbmcremote");
    return settings.value("VolumeOnCall", 10).toInt();
}

void Settings::setShowCallNotifications(bool show)
{
    QSettings settings("xbmcremote");
    settings.setValue("ShowCallNotifications", show);
    emit showCallNotificationsChanged();
}

bool Settings::showCallNotifications()
{
    QSettings settings("xbmcremote");
    return settings.value("ShowCallNotifications", true).toBool();
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
    emit changeVolumeOnCallChanged();
}

bool Settings::pauseVideoOnCall() const
{
    QSettings settings("xbmcremote");
    return settings.value("PauseVideoOnCall", true).toBool();
}

void Settings::setPauseVideoOnCall(bool pause)
{
    QSettings settings("xbmcremote");
    settings.setValue("PauseVideoOnCall", pause);
    emit pauseVideoOnCallChanged();
}

bool Settings::pauseMusicOnCall() const
{
    QSettings settings("xbmcremote");
    return settings.value("PauseMusicOnCall", false).toBool();
}

void Settings::setPauseMusicOnCall(bool pause)
{
    QSettings settings("xbmcremote");
    settings.setValue("PauseMusicOnCall", pause);
    emit pauseMusicOnCallChanged();
}

void Settings::addHost(const XbmcHost &host)
{
    QSettings settings("xbmcremote");
    settings.beginGroup("Hosts");
    settings.beginGroup(host.address());
    settings.setValue("Hostname", host.hostname());
    settings.setValue("Username", host.username());
    settings.setValue("Password", host.password());
    settings.setValue("MAC", host.hwAddr());
    settings.setValue("Port", host.port());
    settings.setValue("VolumeUpCommand", host.volumeUpCommand());
    settings.setValue("VolumeDownCommand", host.volumeDownCommand());
}

void Settings::removeHost(const XbmcHost &host)
{
    QSettings settings("xbmcremote");
    settings.beginGroup("Hosts");
    settings.beginGroup(host.address());
    settings.remove("");
}

QList<XbmcHost> Settings::hostList() const
{
    QList<XbmcHost> list;

    QSettings settings("xbmcremote");
    settings.beginGroup("Hosts");
    foreach(const QString &hostGroup, settings.childGroups()) {
        settings.beginGroup(hostGroup);
        XbmcHost host;
        host.setAddress(hostGroup);
        host.setHostname(settings.value("Hostname").toString());
        host.setHwAddr(settings.value("MAC").toString());
        host.setUsername(settings.value("Username").toString());
        host.setPassword(settings.value("Password").toString());
        host.setPort(settings.value("Port").toInt());
        host.setXbmcHttpSupported(true);
        host.setXbmcJsonrpcSupported(true);
        host.setVolumeUpCommand(settings.value("VolumeUpCommand").toString());
        host.setVolumeDownCommand(settings.value("VolumeDownCommand").toString());
        list.append(host);
        settings.endGroup();
    }
    return list;
}

XbmcHost Settings::lastHost() const
{
    QSettings settings("xbmcremote");
    XbmcHost host;
    if(settings.contains("LastHost")) {
        QString lastHost = settings.value("LastHost").toString();
        settings.beginGroup("Hosts");
        settings.beginGroup(lastHost);
        host.setAddress(lastHost);
        host.setHostname(settings.value("Hostname").toString());
        host.setHwAddr(settings.value("MAC").toString());
        host.setPassword(settings.value("Password").toString());
        host.setPort(settings.value("Port").toInt());
        host.setUsername(settings.value("Username").toString());
        host.setVolumeUpCommand(settings.value("VolumeUpCommand").toString());
        host.setVolumeDownCommand(settings.value("VolumeDownCommand").toString());
        host.setXbmcHttpSupported(true);
        host.setXbmcJsonrpcSupported(true);
    }
    return host;
}

void Settings::setLastHost(const XbmcHost &host)
{
    QSettings settings("xbmcremote");
    settings.setValue("LastHost", host.address());
}

bool Settings::keepDisplayLit() const
{
    QSettings settings("xbmcremote");
    return settings.value("KeepDisplayLit", false).toBool();
}

void Settings::setKeepDisplayLit(bool keepLit)
{
    QSettings settings("xbmcremote");
    settings.setValue("KeepDisplayLit", keepLit);
    emit keepDisplayLitChanged();
}

bool Settings::musicShowsFiles() const
{
    QSettings settings("xbmcremote");
    return settings.value("MusicShowsFiles", false).toBool();
}

void Settings::setMusicShowsFiles(bool showFiles)
{
    QSettings settings("xbmcremote");
    settings.setValue("MusicShowsFiles", showFiles);
    emit musicShowsFilesChanged();
}

bool Settings::videoShowsFiles() const
{
    QSettings settings("xbmcremote");
    return settings.value("VideoShowsFiles", false).toBool();
}

void Settings::setVideoShowsFiles(bool showFiles)
{
    QSettings settings("xbmcremote");
    settings.setValue("VideoShowsFiles", showFiles);
    emit videoShowsFilesChanged();
}
