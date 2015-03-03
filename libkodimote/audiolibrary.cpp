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

#include "audiolibrary.h"
#include "artists.h"
#include "albums.h"
#include "songs.h"
#include "genres.h"
#include "recentitems.h"
#include "kodiconnection.h"
#include "libraryitem.h"
#include "addonsource.h"
#include "shares.h"
#include "playlists.h"

AudioLibrary::AudioLibrary() :
    KodiLibrary(0)
{
    setBusy(false);
    LibraryItem *item = new LibraryItem(tr("Artists"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Albums"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Titles"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Genres"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Recently added"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Recently played"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Playlists"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Music Add-ons"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Files"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);
}

KodiModel *AudioLibrary::enterItem(int index)
{
    switch(index) {
    case 0:
        return new Artists(-1, this);
    case 1:
        return new Albums(-1, -1, this);
    case 2:
        return new Songs(-1, -1, this);
    case 3:
        return new Genres(this);
    case 4:
        return new RecentItems(RecentItems::ModeAudio, RecentItems::RecentlyAdded, this);
    case 5:
        return new RecentItems(RecentItems::ModeAudio, RecentItems::RecentlyPlayed, this);
    case 6:
        return new Playlists(tr("Audio playlists"), "music", "special://musicplaylists", this);
    case 7:
        return new AddonSource(tr("Music Add-ons"), "music", "addons://sources/audio", this);
    case 8:
        return new Shares("music");
    }
    return 0;
}

void AudioLibrary::playItem(int index, bool resume)
{
    Q_UNUSED(index)
    Q_UNUSED(resume)
    qDebug() << "cannot play whole audio library";
}

void AudioLibrary::addToPlaylist(int index)
{
    Q_UNUSED(index)
}

QString AudioLibrary::title() const
{
    return tr("Audio Library");
}

bool AudioLibrary::allowSearch()
{
    return false;
}

void AudioLibrary::scanForContent()
{
    KodiConnection::sendCommand("AudioLibrary.Scan");
}

void AudioLibrary::clean()
{
    KodiConnection::sendCommand("AudioLibrary.Clean");
}

void AudioLibrary::refresh()
{
    // nothing to do here...
}
