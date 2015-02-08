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

#include "videolibrary.h"
#include "movies.h"
#include "tvshows.h"
#include "musicvideos.h"
#include "recentitems.h"
#include "kodiconnection.h"
#include "libraryitem.h"
#include "addonsource.h"
#include "shares.h"

VideoLibrary::VideoLibrary(KodiModel *parent) :
    KodiLibrary(parent)
{
    setBusy(false);
    LibraryItem *item = new LibraryItem(tr("Movies"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("TV Shows"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Music Videos"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Recently added"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Video Add-ons"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Files"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);
}

KodiModel *VideoLibrary::enterItem(int index)
{
    switch(index) {
    case 0:
        return new Movies(false, this);
    case 1:
        return new TvShows(this);
    case 2:
        return new MusicVideos(false, this);
    case 3:
        return new RecentItems(MediaFormatVideo, RecentItems::RecentlyAdded, this);
    case 4:
        return new AddonSource(tr("Video Add-ons"), "video", "addons://sources/video", this);
    case 5:
        return new Shares("video");
    }
    return 0;
}

void VideoLibrary::playItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "cannot play whole video library";
}

void VideoLibrary::addToPlaylist(int index)
{
    Q_UNUSED(index)
}

QString VideoLibrary::title() const
{
    return tr("Video Library");
}

bool VideoLibrary::allowSearch()
{
    return false;
}

void VideoLibrary::scanForContent()
{
    KodiConnection::sendCommand("VideoLibrary.Scan");
}

void VideoLibrary::clean()
{
    KodiConnection::sendCommand("VideoLibrary.Clean");
}

void VideoLibrary::refresh()
{
    // Nothing to do here
}
