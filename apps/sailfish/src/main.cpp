/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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

#include <QDir>
#include <QtQuick>
#include <QStandardPaths>

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/eventclient.h"
#include "libxbmcremote/settings.h"
#include "libxbmcremote/networkaccessmanagerfactory.h"
#include "sailfishhelper.h"

#include <sailfishapp.h>

int main(int argc, char *argv[])
{
    QGuiApplication *application = SailfishApp::application(argc, argv);

    // Load language file
    QString language = QLocale::system().bcp47Name();
    qDebug() << "got language:" << language;

    QTranslator qtTranslator;
    if(!qtTranslator.load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        qDebug() << "couldn't load qt_" + language;
    }
    application->installTranslator(&qtTranslator);

    QTranslator translator;
    if (!translator.load(":/xbmcremote_" + language + ".qm")) {
        qDebug() << "Cannot load translation file" << "xbmcremote_" + language + ".qm";
    }
    application->installTranslator(&translator);

    Xbmc::instance()->setDataPath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));

    Settings settings;

    SailfishHelper helper(&settings);
    Q_UNUSED(helper)

    QQuickView *view = SailfishApp::createView();
    view->engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory());
    view->engine()->rootContext()->setContextProperty("xbmc", Xbmc::instance());
    view->engine()->rootContext()->setContextProperty("settings", &settings);
    view->setSource(SailfishApp::pathTo("qml/main.qml"));

    view->show();

    return application->exec();
}

