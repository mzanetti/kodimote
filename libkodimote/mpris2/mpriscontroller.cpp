#include "mpriscontroller.h"

#include <QGuiApplication>
#include <QtDBus/QtDBus>

#include "mprisapplication.h"
#include "mprisplayer.h"

MprisController::MprisController(QObject *parent) :
    QObject(parent)
{
    QDBusConnection bus = QDBusConnection::sessionBus();

    bus.registerService("org.mpris.MediaPlayer2.kodimote");
    new MprisPlayer(this);
    new MprisApplication(this);

    bus.registerObject("/org/mpris/MediaPlayer2", this);
}
