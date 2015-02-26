#include "nativeprotocolhandler.h"

#include "../kodi.h"
#include "../videoplayer.h"

NativeProtocolHandler::NativeProtocolHandler(const QString &scheme, QObject *parent) :
    ProtocolHandler(parent),
    m_scheme(scheme)
{
}

QString NativeProtocolHandler::scheme() const
{
    return m_scheme;
}

void NativeProtocolHandler::execute(const QUrl &uri, bool queue)
{
    Q_UNUSED(queue)
    PlaylistItem item;
    item.setFile(uri.toString());

    //we don't know the type of medium, that's why we don't support adding to a playlist,
    //and can't use the audioPlayer as it always adds to the playlist and thus also depends on the type
    //TODO: investigate if we realy need different playlists, and can't just queue without playlistId
    Kodi::instance()->videoPlayer()->open(item);
}

void NativeProtocolHandler::registerAll(ProtocolManager *manager)
{
    manager->registerProtocol(new NativeProtocolHandler("http", manager));
    manager->registerProtocol(new NativeProtocolHandler("https", manager));
    manager->registerProtocol(new NativeProtocolHandler("smb", manager));
    manager->registerProtocol(new NativeProtocolHandler("sftp", manager));
    manager->registerProtocol(new NativeProtocolHandler("nfs", manager));
    manager->registerProtocol(new NativeProtocolHandler("upnp", manager));
    manager->registerProtocol(new NativeProtocolHandler("plugin", manager));
}
