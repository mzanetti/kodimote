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

#include "playerjob.h"

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/player.h"

#include <QDebug>

PlayerJob::PlayerJob(const QString& operation, QMap<QString,QVariant>& parameters, QObject* parent):
    Plasma::ServiceJob(operation, operation, parameters, parent)
{

}

void PlayerJob::start()
{
    qDebug() << "should start job:" << operationName();

    Player *activePlayer = Xbmc::instance()->activePlayer();

    if(!activePlayer) {
        return;
    }

    if(operationName() == "PlayPause") {
        activePlayer->playPause();
    } else if(operationName() == "Stop") {
        activePlayer->stop();
    } else if(operationName() == "SkipNext") {
        activePlayer->skipNext();
    } else if(operationName() == "SkipPrevious") {
        activePlayer->skipPrevious();
    } else if(operationName() == "SeekForward") {
        activePlayer->seekForward();
    } else if(operationName() == "SeekBackward") {
        activePlayer->seekBackward();
    } else if (operationName() == "Seek") {
        activePlayer->seek(parameters().value("position").toDouble());
    }
}
