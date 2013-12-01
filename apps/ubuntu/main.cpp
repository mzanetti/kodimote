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
#include "libxbmcremote/eventclient.h"
#include "libxbmcremote/networkaccessmanagerfactory.h"

#include "ubuntuhelper.h"

#include <QGuiApplication>

#include <QDebug>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDir>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char** argv)
{

    QCoreApplication::setOrganizationName("com.ubuntu.developer.mzanetti.xbmcremote");
    QCoreApplication::setApplicationName("xbmcremote");

    QGuiApplication application(argc, argv);

    // Load language file
    QString language = QLocale::system().bcp47Name();
    qDebug() << "got language:" << language;

    QTranslator qtTranslator;

    if(!qtTranslator.load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        qDebug() << "couldn't load qt_" + language;
    }
    application.installTranslator(&qtTranslator);

    QTranslator translator;
    if (!translator.load(":/xbmcremote_" + language + ".qm")) {
        qDebug() << "Cannot load translation file" << "xbmcremote_" + language + ".pm";
    }
    application.installTranslator(&translator);


    Xbmc::instance()->setDataPath(QDir::homePath() + "/.cache/com.ubuntu.developer.mzanetti.xbmcremote/");
    Xbmc::instance()->eventClient()->setApplicationThumbnail("xbmcremote80.png");

    QQuickView *view = new QQuickView();
    view->setResizeMode(QQuickView::SizeRootObjectToView);

    view->engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory());

    view->setTitle("Xbmcremote");
    view->engine()->rootContext()->setContextProperty("xbmc", Xbmc::instance());


    Settings settings;
    view->engine()->rootContext()->setContextProperty("settings", &settings);

    if(QCoreApplication::applicationDirPath() == QDir(("/usr/bin")).canonicalPath()) {
        view->setSource(QUrl::fromLocalFile("/usr/share/xbmcremote/qml/main.qml"));
    } else {
        view->setSource(QUrl::fromLocalFile("qml/main.qml"));
    }

    UbuntuHelper helper(&settings);
    Q_UNUSED(helper);

    if(QGuiApplication::arguments().contains("--fullscreen")) {
        view->showFullScreen();
    } else {
        view->resize(QSize(720, 1280));
        view->show();
    }

    return application.exec();
}

