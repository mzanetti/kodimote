#include "mprisapplication.h"

#include "kodi.h"
#include "kodihost.h"
#include "keys.h"

MprisApplication::MprisApplication(ProtocolManager *protocols, QObject *parent) :
    QDBusAbstractAdaptor(parent),
    m_protocols(protocols)
{
}

bool MprisApplication::canQuit() const
{
    return true;
}

bool MprisApplication::canSetFullscreen() const
{
    return false;
}

bool MprisApplication::canRaise() const
{
    return false;
}

bool MprisApplication::hasTrackList() const
{
    return false;
}

QString MprisApplication::identity() const
{
    KodiHost *host = Kodi::instance()->connectedHost();
    if (host) {
        return tr("Kodi on %1").arg(host->hostname());
    } else {
        return "Kodimote";
    }
}

QString MprisApplication::desktopEntry() const
{
#ifdef SAILFISH
    return "harbour-kodimote";
#else
    return "kodimote";
#endif
}

QStringList MprisApplication::supportedUriSchemes() const
{
    QStringList schemes;

    foreach (ProtocolHandler *protocol, m_protocols->list()) {
        schemes.append(protocol->scheme());
    }

    return schemes;
}

QStringList MprisApplication::supportedMimeTypes() const
{
    QStringList mimeTypes;
    return mimeTypes;
}

bool MprisApplication::fullscreen() const
{
    return true;
}

void MprisApplication::setFullscreen(bool fullscreen)
{
    Q_UNUSED(fullscreen)
}

void MprisApplication::Quit()
{
    Kodi::instance()->quit();
}

void MprisApplication::Raise()
{
}
