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

#include "youtubeprotocolhandler.h"

#include <QUrlQuery>
#include "kodi.h"
#include "videoplayer.h"
#include "playlist.h"

YoutubeProtocolHandler::YoutubeProtocolHandler(QObject *parent) :
    ProtocolHandler(parent)
{
}

QString YoutubeProtocolHandler::scheme() const
{
    return "youtube";
}

void YoutubeProtocolHandler::execute(const QUrl &uri, bool queue)
{
    QStringList parts = uri.path().split('/');
    QUrlQuery query;
    if (parts.length() == 1) {
        query.addQueryItem("action", "play_video");
        query.addQueryItem("videoid", parts[0]);
    } else if (parts.length() == 2) {
        if (parts[0] == "video") {
            query.addQueryItem("action", "play_video");
            query.addQueryItem("videoid", parts[1]);
        }
    }

    if (query.isEmpty()) {
        return;
    }

    PlaylistItem item;
    item.setFile("plugin://plugin.video.youtube/?" + query.toString());

    Player *player = Kodi::instance()->videoPlayer();
    if (queue) {
        player->playlist()->addItems(item);
    } else {
        player->open(item);
    }
}
