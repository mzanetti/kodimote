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

    // Qt5 final - enable when ubuntu switches to final
    view->setTitle("Xbmcremote");

    // Qt5 beta 1 - remove when ubuntu switches to final
//    view->setWindowTitle("Xbmcremote");

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
    host.setAddress("10.10.10.100");
    host.setHostname("Knuckles");
    host.setPort(8080);
    Xbmc::instance()->hostModel()->insertOrUpdateHost(host);
    Xbmc::instance()->hostModel()->connectToHost(0);

    return application.exec();
}

