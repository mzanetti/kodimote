/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014-2015 Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Kodimote                                             *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "platformhelper.h"

#include "kodi.h"
#include "settings.h"
#include "videoplayer.h"
#include "audioplayer.h"

PlatformHelper::PlatformHelper(Settings *settings, QObject *parent) :
    QObject(parent),
    m_settings(settings),
    m_musicPaused(false),
    m_videoPaused(false)
{
}

void PlatformHelper::callStarted(bool incoming, const QString &caller)
{
    Kodi *kodi = Kodi::instance();
    if (incoming && m_settings->showCallNotifications()) {
        kodi->sendNotification(tr("Incoming call"), caller);
    }

    if(m_settings->changeVolumeOnCall()) {
        kodi->dimVolumeTo(m_settings->volumeOnCall());
    }

    if(m_settings->pauseVideoOnCall() && kodi->videoPlayer()->state() == "playing") {
        kodi->videoPlayer()->playPause();
        m_videoPaused = true;
    }

    if(m_settings->pauseMusicOnCall() && kodi->audioPlayer()->state() == "playing") {
        kodi->audioPlayer()->playPause();
        m_musicPaused = true;
    }
}

void PlatformHelper::callEnded()
{
    if(m_settings->changeVolumeOnCall()) {
        Kodi::instance()->restoreVolume();
    }

    if(m_videoPaused) {
        Kodi::instance()->videoPlayer()->playPause();
        m_videoPaused = false;
    }
    if(m_musicPaused) {
        Kodi::instance()->audioPlayer()->playPause();
        m_musicPaused = false;
    }
}
