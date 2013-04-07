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

#include "playercontainer.h"

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/player.h"
#include "libxbmcremote/audioplayer.h"
#include "libxbmcremote/videoplayer.h"
#include "libxbmcremote/libraryitem.h"

#include <QDebug>


PlayerContainer::PlayerContainer(QObject *parent):
    Plasma::DataContainer(parent),
    m_player(0)
{

    connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(activePlayerChanged()));
    connect(Xbmc::instance(), SIGNAL(activePlayerChanged()), SLOT(activePlayerChanged()));

    activePlayerChanged();
}

void PlayerContainer::activePlayerChanged()
{

    disconnect(this, SLOT(update()));
    disconnect(this, SLOT(currentItemChanged()));
    disconnect(this, SLOT(percentageChanged()));

    if(Xbmc::instance()->activePlayer()) {
        connect(Xbmc::instance()->activePlayer(), SIGNAL(stateChanged()), SLOT(update()));
        connect(Xbmc::instance()->activePlayer(), SIGNAL(currentItemChanged()), SLOT(currentItemChanged()));
        connect(Xbmc::instance()->activePlayer(), SIGNAL(percentageChanged()), SLOT(percentageChanged()));

        Xbmc::instance()->activePlayer()->setTimerActive(true);
    }

    update();
}

void PlayerContainer::currentItemChanged()
{
    if(!Xbmc::instance()->activePlayer()) {
        removeAllData();
        return;
    }

    disconnect(this, SLOT(thumbnailChanged()));

    LibraryItem *currentItem = Xbmc::instance()->activePlayer()->currentItem();
    connect(currentItem, SIGNAL(thumbnailChanged()), SLOT(thumbnailChanged()));

    setData("title", currentItem->title());
    setData("year", currentItem->year());
    setData("rating", currentItem->rating());
    setData("duration", currentItem->duration());
    setData("durationInSecs", currentItem->durationInSecs());
    setData("durationString", currentItem->durationString());

    // Need to call the others too, but with only one checkForUpate() :/
    setData("thumbnail", Xbmc::instance()->activePlayer()->currentItem()->thumbnail());
    setData("percentage", Xbmc::instance()->activePlayer()->percentage());
    setData("time", Xbmc::instance()->activePlayer()->time());

    checkForUpdate();
}

void PlayerContainer::percentageChanged()
{
    setData("percentage", Xbmc::instance()->activePlayer()->percentage());
    setData("time", Xbmc::instance()->activePlayer()->time());
    checkForUpdate();
}

void PlayerContainer::thumbnailChanged()
{
    setData("thumbnail", Xbmc::instance()->activePlayer()->currentItem()->thumbnail());
    checkForUpdate();
}

void PlayerContainer::update()
{
    qDebug() << "updated!!!!";
    m_player = Xbmc::instance()->activePlayer();

    if(m_player == 0) {
        setData("state", "none");
        checkForUpdate();
    } else {
        setData("state", m_player->state());
        currentItemChanged();
    }
}
