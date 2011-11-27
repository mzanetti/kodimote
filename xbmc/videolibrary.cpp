/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
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
#include "xbmcconnection.h"

VideoLibrary::VideoLibrary(XbmcModel *parent) :
    XbmcLibrary(parent)
{
    setBusy(false);
    LibraryItem *item = new LibraryItem("Movies");
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem("TV Shows");
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem("Music Videos");
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);
}

XbmcModel *VideoLibrary::enterItem(int index)
{
    switch(index) {
    case 0:
        return new Movies(this);
    case 1:
        return new TvShows(this);
    case 2:
        return new MusicVideos(this);
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
    return "Video Library";
}

void VideoLibrary::scanForContent()
{
    XbmcConnection::sendCommand("VideoLibrary.Scan");
}
