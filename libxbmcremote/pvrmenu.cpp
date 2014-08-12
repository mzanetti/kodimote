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

#include "pvrmenu.h"

#include "libraryitem.h"

#include "channelgroups.h"
#include "recordings.h"

PvrMenu::PvrMenu(XbmcModel *parent) :
    XbmcLibrary(parent)
{
    setBusy(false);
    LibraryItem *item = new LibraryItem(tr("TV Channels"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);

    item = new LibraryItem(tr("Recordings"), QString(), this);
    item->setFileType("directory");
    item->setPlayable(false);
    m_list.append(item);
}

QString PvrMenu::title() const
{
    return tr("Live TV");
}

XbmcModel *PvrMenu::enterItem(int index)
{
    switch (index) {
    case 0:
        return new ChannelGroups(this);
    case 1:
        return new Recordings("", false, this);
    }

    return 0;
}

void PvrMenu::playItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "cannot play PVR Menu";
}

void PvrMenu::addToPlaylist(int index)
{
    Q_UNUSED(index);
}

void PvrMenu::refresh()
{

}
