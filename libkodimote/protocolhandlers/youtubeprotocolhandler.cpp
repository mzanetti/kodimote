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
