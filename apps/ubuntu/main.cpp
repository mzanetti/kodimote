/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "libkodimote/kodi.h"
#include "libkodimote/settings.h"
#include "libkodimote/eventclient.h"
#include "libkodimote/networkaccessmanagerfactory.h"
#include "libkodimote/mpris2/mpriscontroller.h"

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

    QCoreApplication::setOrganizationName("com.ubuntu.developer.mzanetti.kodimote");
    QCoreApplication::setApplicationName("kodimote");

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
    if (!translator.load(":/kodimote_" + language + ".qm")) {
        qDebug() << "Cannot load translation file" << "kodimote_" + language + ".pm";
    }
    application.installTranslator(&translator);


    Kodi::instance()->setDataPath(QDir::homePath() + "/.cache/com.ubuntu.developer.mzanetti.kodimote/");
    Kodi::instance()->eventClient()->setApplicationThumbnail("kodimote80.png");

    QQuickView *view = new QQuickView();

    Settings settings;
    UbuntuHelper helper(view, &settings);

    ProtocolManager protocols;

    MprisController controller(&protocols, &helper);
    Q_UNUSED(controller)


    view->setResizeMode(QQuickView::SizeRootObjectToView);

    view->engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory());

    view->setTitle("Kodimote");
    view->engine()->rootContext()->setContextProperty("kodi", Kodi::instance());

    view->engine()->rootContext()->setContextProperty("settings", &settings);
    view->engine()->rootContext()->setContextProperty("protocolManager", &protocols);
    view->setSource(QUrl("qrc:///qml/main.qml"));

    if(QGuiApplication::arguments().contains("--fullscreen")) {
        view->showFullScreen();
    } else {
        view->resize(QSize(720, 1280));
        view->show();
    }

    return application.exec();
}

