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

#include "audiolibrary.h"
#include "artists.h"
#include "albums.h"
#include "songs.h"
#include "xbmcconnection.h"
#include "libraryitem.h"

AudioLibrary::AudioLibrary() :
    XbmcLibrary(0)
{
    setBusy(false);
    LibraryItem *item = new LibraryItem(tr("Artists"));
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Albums"));
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Titles"));
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);
}

XbmcModel *AudioLibrary::enterItem(int index)
{
    switch(index) {
    case 0:
        return new Artists(this);
    case 1:
        return new Albums(-1, this);
    case 2:
        return new Songs(-1, -1, this);
    }
    return 0;
}

void AudioLibrary::playItem(int index)
{
    Q_UNUSED(index)
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
    XbmcConnection::sendCommand("AudioLibrary.Scan");
}

void AudioLibrary::clean()
{
    XbmcConnection::sendCommand("AudioLibrary.Clean");
}

void AudioLibrary::refresh()
{
    // nothing to do here...
}