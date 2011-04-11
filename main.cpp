#include "xbmc.h"

#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include <QDeclarativeContext>

#include "xbmcconnection.h"
#include "audioplayer.h"
#include "audioplaylist.h"
#include "videoplayer.h"
#include "audiolibrary.h"

int main(int argc, char *argv[])
{
    qDebug() << "starting up";
    QApplication app(argc, argv);

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);

    qDebug() << "connecting xbmc";
    Xbmc::XbmcConnection::connect("10.10.10.100", 9090);

    Xbmc::Xbmc xbmc;
    viewer.rootContext()->setContextProperty("Xbmc", &xbmc);

    viewer.rootContext()->setContextProperty("AudioPlayer", xbmc.audioPlayer());
    viewer.rootContext()->setContextProperty("AudioPlaylist", xbmc.audioPlayer()->playlist());
    viewer.rootContext()->setContextProperty("AudioLibrary", xbmc.audioLibrary());

    viewer.rootContext()->setContextProperty("VideoPlayer", xbmc.videoPlayer());

    viewer.setMainQmlFile(QLatin1String("qml/xbmcremote/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
