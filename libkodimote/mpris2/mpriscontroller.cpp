#include "mpriscontroller.h"

#include <QGuiApplication>
#include <QtDBus/QtDBus>

#include "mprisapplication.h"
#include "mprisplayer.h"

MprisController::MprisController(ProtocolManager *protocols, QObject *parent) :
    QObject(parent)
{
    QDBusConnection bus = QDBusConnection::sessionBus();

    bus.registerService("org.mpris.MediaPlayer2.kodimote");
    new MprisPlayer(protocols, this);
    new MprisApplication(protocols, this);

    bus.registerObject("/org/mpris/MediaPlayer2", this);
}
