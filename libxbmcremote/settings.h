/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H


#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool themeInverted READ themeInverted WRITE setThemeInverted NOTIFY themeInvertedChanged)
    Q_PROPERTY(bool useThumbnails READ useThumbnails WRITE setUseThumbnails NOTIFY useThumbnailsChanged)
    Q_PROPERTY(bool ignoreArticle READ ignoreArticle WRITE setIgnoreArticle NOTIFY ignoreArticleChanged)
    Q_PROPERTY(bool changeVolumeOnCall READ changeVolumeOnCall WRITE setChangeVolumeOnCall NOTIFY changeVolumeOnCallChanged)
    Q_PROPERTY(int volumeOnCall READ volumeOnCall WRITE setVolumeOnCall NOTIFY volumeOnCallChanged)
    Q_PROPERTY(bool pauseVideoOnCall READ pauseVideoOnCall WRITE setPauseVideoOnCall NOTIFY pauseVideoOnCallChanged)
    Q_PROPERTY(bool pauseMusicOnCall READ pauseMusicOnCall WRITE setPauseMusicOnCall NOTIFY pauseMusicOnCallChanged)
    Q_PROPERTY(bool keepDisplayLit READ keepDisplayLit WRITE setKeepDisplayLit NOTIFY keepDisplayLitChanged)
    Q_PROPERTY(bool showCallNotifications READ showCallNotifications WRITE setShowCallNotifications NOTIFY showCallNotificationsChanged)
    Q_PROPERTY(bool musicShowsFiles READ musicShowsFiles WRITE setMusicShowsFiles NOTIFY musicShowsFilesChanged)
    Q_PROPERTY(bool videoShowsFiles READ videoShowsFiles WRITE setVideoShowsFiles NOTIFY videoShowsFilesChanged)
    Q_PROPERTY(bool musicEnabled READ musicEnabled WRITE setMusicEnabled NOTIFY musicEnabledChanged)
    Q_PROPERTY(bool videosEnabled READ videosEnabled WRITE setVideosEnabled NOTIFY videosEnabledChanged)
    Q_PROPERTY(bool picturesEnabled READ picturesEnabled WRITE setPicturesEnabled NOTIFY picturesEnabledChanged)
    Q_PROPERTY(bool pvrEnabled READ pvrEnabled WRITE setPvrEnabled NOTIFY pvrEnabledChanged)
    Q_PROPERTY(bool hapticsEnabled READ hapticsEnabled WRITE setHapticsEnabled NOTIFY hapticsEnabledChanged)

public:
    explicit Settings(QObject *parent = 0);

    void setThemeInverted(bool inverted);
    bool themeInverted() const;

    void setUseThumbnails(bool useThumbnails);
    bool useThumbnails() const;

    void setIgnoreArticle(bool ignoreArticle);
    bool ignoreArticle() const;

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

    bool keepDisplayLit() const;
    void setKeepDisplayLit(bool keepLit);

    bool musicShowsFiles() const;
    void setMusicShowsFiles(bool showFiles);

    bool videoShowsFiles() const;
    void setVideoShowsFiles(bool showFiles);

    bool musicEnabled() const;
    void setMusicEnabled(bool enabled);

    bool videosEnabled() const;
    void setVideosEnabled(bool enabled);

    bool picturesEnabled() const;
    void setPicturesEnabled(bool enabled);

    bool pvrEnabled() const;
    void setPvrEnabled(bool enabled);

    bool hapticsEnabled() const;
    void setHapticsEnabled(bool enabled);

signals:
    void themeInvertedChanged();
    void useThumbnailsChanged();
    void ignoreArticleChanged();
    void volumeUpCommandChanged();
    void volumeDownCommandChanged();
    void changeVolumeOnCallChanged();
    void volumeOnCallChanged();
    void pauseVideoOnCallChanged();
    void pauseMusicOnCallChanged();
    void keepDisplayLitChanged();
    void showCallNotificationsChanged();
    void musicShowsFilesChanged();
    void videoShowsFilesChanged();
    void musicEnabledChanged();
    void videosEnabledChanged();
    void picturesEnabledChanged();
    void pvrEnabledChanged();
    void hapticsEnabledChanged();
};

#endif // SETTINGS_H
