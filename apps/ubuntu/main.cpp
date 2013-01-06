#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/settings.h"

#include <QGuiApplication>

#include <QDebug>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDir>

int main(int argc, char** argv)
{
    QGuiApplication::setApplicationName("Xbmcremote");

    QGuiApplication application(argc, argv);

    QQuickView *view = new QQuickView();
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setWindowTitle("Xbmcremote");

    view->engine()->rootContext()->setContextProperty("xbmc", Xbmc::instance());

    Settings settings;
    view->engine()->rootContext()->setContextProperty("settings", &settings);

    if(QCoreApplication::applicationDirPath() == QDir(("/usr/bin")).canonicalPath()) {
        view->setSource(QUrl::fromLocalFile("/usr/share/xbmcremote/qml/main.qml"));
    } else {
        view->setSource(QUrl::fromLocalFile("qml/main.qml"));
    }

    if(QGuiApplication::arguments().contains("--fullscreen")) {
        view->showFullScreen();
    } else {
        view->resize(QSize(720, 1280));
        view->show();
    }

    // Just connect to my host for now
    XbmcHost host;
    host.setAddress("10.10.10.10");
    host.setHostname("Knuckles");
    host.setPort(8080);
    Xbmc::instance()->hostModel()->insertOrUpdateHost(host);
    Xbmc::instance()->hostModel()->connectToHost(0);

    return application.exec();
}

