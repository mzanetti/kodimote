/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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

#include <QDir>
#include <QtQuick>
#include <QStandardPaths>

#ifndef HARBOUR_BUILD
#include <grp.h>
#include <pwd.h>
#endif

#include "libkodimote/kodi.h"
#include "libkodimote/eventclient.h"
#include "libkodimote/settings.h"
#include "libkodimote/networkaccessmanagerfactory.h"
#include "sailfishhelper.h"

#include <sailfishapp.h>

int main(int argc, char *argv[])
{
#ifndef HARBOUR_BUILD
    setuid(getpwnam("nemo")->pw_uid);
    setgid(getgrnam("privileged")->gr_gid);
#endif

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
    if (!translator.load(":/kodimote_" + language + ".qm")) {
        qDebug() << "Cannot load translation file" << "kodimote_" + language + ".qm";
    }
    application->installTranslator(&translator);

    Kodi::instance()->setDataPath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));

    Settings settings;

    SailfishHelper helper(&settings);
    Q_UNUSED(helper)

    QQuickView *view = SailfishApp::createView();
    view->engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory());
    view->engine()->rootContext()->setContextProperty("kodi", Kodi::instance());
    view->engine()->rootContext()->setContextProperty("settings", &settings);
    view->setSource(SailfishApp::pathTo("qml/main.qml"));

    view->show();

    return application->exec();
}

