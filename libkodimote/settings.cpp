/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "settings.h"

#include <QSettings>
#include <QStringList>
#include <QCoreApplication>

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

bool Settings::themeInverted() const
{
    QSettings settings;
    return settings.value("InvertTheme", true).toBool();
}

void Settings::setThemeInverted(bool inverted)
{
    QSettings settings;
    settings.setValue("InvertTheme", inverted);
    emit themeInvertedChanged();
}

bool Settings::useThumbnails() const
{
    QSettings settings;
    return settings.value("UseThumbnails", true).toBool();
}

void Settings::setIgnoreArticle(bool ignoreArticle)
{
    QSettings settings;
    settings.setValue("IgnoreArticle", ignoreArticle);
    emit ignoreArticleChanged();
}

bool Settings::ignoreArticle() const
{
    QSettings settings;
    return settings.value("IgnoreArticle", true).toBool();
}

void Settings::setUseThumbnails(bool useThumbnails)
{
    QSettings settings;
    settings.setValue("UseThumbnails", useThumbnails);
    emit useThumbnailsChanged();
}


void Settings::setVolumeOnCall(int volume)
{
    QSettings settings;
    settings.setValue("VolumeOnCall", volume);
    emit volumeOnCallChanged();
}

int Settings::volumeOnCall() const
{
    QSettings settings;
    return settings.value("VolumeOnCall", 10).toInt();
}

void Settings::setShowCallNotifications(bool show)
{
    QSettings settings;
    settings.setValue("ShowCallNotifications", show);
    emit showCallNotificationsChanged();
}

bool Settings::showCallNotifications()
{
    QSettings settings;
    return settings.value("ShowCallNotifications", true).toBool();
}

bool Settings::changeVolumeOnCall() const
{
    QSettings settings;
    return settings.value("ChangeVolumeOnCall", true).toBool();
}

void Settings::setChangeVolumeOnCall(bool changeVolume)
{
    QSettings settings;
    settings.setValue("ChangeVolumeOnCall", changeVolume);
    emit changeVolumeOnCallChanged();
}

bool Settings::pauseVideoOnCall() const
{
    QSettings settings;
    return settings.value("PauseVideoOnCall", true).toBool();
}

void Settings::setPauseVideoOnCall(bool pause)
{
    QSettings settings;
    settings.setValue("PauseVideoOnCall", pause);
    emit pauseVideoOnCallChanged();
}

bool Settings::pauseMusicOnCall() const
{
    QSettings settings;
    return settings.value("PauseMusicOnCall", false).toBool();
}

void Settings::setPauseMusicOnCall(bool pause)
{
    QSettings settings;
    settings.setValue("PauseMusicOnCall", pause);
    emit pauseMusicOnCallChanged();
}

bool Settings::keepDisplayLit() const
{
    QSettings settings;
    return settings.value("KeepDisplayLit", false).toBool();
}

void Settings::setKeepDisplayLit(bool keepLit)
{
    QSettings settings;
    settings.setValue("KeepDisplayLit", keepLit);
    emit keepDisplayLitChanged();
}

bool Settings::musicShowsFiles() const
{
    QSettings settings;
    return settings.value("MusicShowsFiles", false).toBool();
}

void Settings::setMusicShowsFiles(bool showFiles)
{
    QSettings settings;
    settings.setValue("MusicShowsFiles", showFiles);
    emit musicShowsFilesChanged();
}

bool Settings::videoShowsFiles() const
{
    QSettings settings;
    return settings.value("VideoShowsFiles", false).toBool();
}

void Settings::setVideoShowsFiles(bool showFiles)
{
    QSettings settings;
    settings.setValue("VideoShowsFiles", showFiles);
    emit videoShowsFilesChanged();
}

bool Settings::musicEnabled() const
{
    QSettings settings;
    return settings.value("MusicEnabled", true).toBool();
}

void Settings::setMusicEnabled(bool enabled)
{
    QSettings settings;
    settings.setValue("MusicEnabled", enabled);
    emit musicEnabledChanged();
}

bool Settings::videosEnabled() const
{
    QSettings settings;
    return settings.value("VideosEnabled", true).toBool();
}

void Settings::setVideosEnabled(bool enabled)
{
    QSettings settings;
    settings.setValue("VideosEnabled", enabled);
    emit videosEnabledChanged();
}

bool Settings::picturesEnabled() const
{
    QSettings settings;
    return settings.value("PicturesEnabled", true).toBool();
}

void Settings::setPicturesEnabled(bool enabled)
{
    QSettings settings;
    settings.setValue("PicturesEnabled", enabled);
    emit picturesEnabledChanged();
}

bool Settings::pvrEnabled() const
{
    QSettings settings;
    return settings.value("PvrEnabled", true).toBool();
}

void Settings::setPvrEnabled(bool enabled)
{
    QSettings settings;
    settings.setValue("PvrEnabled", enabled);
    emit pvrEnabledChanged();
}

bool Settings::hapticsEnabled() const
{
    QSettings settings;
    return settings.value("HapticsEnabled", true).toBool();
}

void Settings::setHapticsEnabled(bool enabled)
{
    QSettings settings;
    settings.setValue("HapticsEnabled", enabled);
    emit hapticsEnabledChanged();
}
