/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xdebug.h"
#include "libxbmcremote/networkaccessmanagerfactory.h"
#include "libxbmcremote/settings.h"

#if defined Q_WS_MAEMO_5 || defined Q_WS_X11
#include "ui/mainwindow.h"
#endif

#if defined Q_WS_S60
#include "symbianhelper.h"
#endif

#if defined Q_WS_S60 || defined Q_WS_SIMULATOR
#include "nfchandler.h"
#include "rumbleeffect.h"
#include "gesturehelper.h"
#endif

#include "qmlapplicationviewer/qmlapplicationviewer.h"
#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QScopedPointer>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QString language;

// Load language file depending on platform
#if defined Q_WS_MAEMO_5
    QApplication *app = new QApplication( argc, argv );
    language = QString(getenv("LC_NAME")).split('_').first();
    qDebug() << "language:" << language << getenv("LANG") << getenv("LC_NAME");
#else
    QApplication *app = new QApplication( argc, argv );
    language = QLocale::system().name();
//    language = QString(getenv("LC_NAME")).split('_').first();
#endif


    QTranslator qtTranslator;
    if(!qtTranslator.load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        qDebug() << "couldn't load qt_" + language;
    }
    app->installTranslator(&qtTranslator);

    QTranslator translator;
    if (!translator.load(":/xbmcremote_" + language + ".qm")) {
        qDebug() << "Cannot load translation file" << "xbmcremote_" + language + ".pm";
    }
    app->installTranslator(&translator);

    // Load debug stuff
    XDebug::addAllowedArea(XDAREA_GENERAL);
    for(int i = 1; i < app->arguments().count(); ++i ) {
        if(app->arguments().at(i) == "-d") {
            if(app->arguments().count() > i) {
                QStringList debuglist = app->arguments().at(i + 1).split(',');
                foreach(const QString &debugString, debuglist) {
                    if(debugString == "connection") {
                        XDebug::addAllowedArea(XDAREA_CONNECTION);
                    } else if(debugString == "player") {
                        XDebug::addAllowedArea(XDAREA_PLAYER);
                    } else if(debugString == "library") {
                        XDebug::addAllowedArea(XDAREA_LIBRARY);
                    } else if(debugString == "files") {
                        XDebug::addAllowedArea(XDAREA_FILES);
                    } else if(debugString == "playlist") {
                        XDebug::addAllowedArea(XDAREA_PLAYLIST);
//                    } else if(debugString == "") {
//                        XDebug::addAllowedArea(XDAREA_);
                    }
                }
            }
        }
    }

    Settings settings;

    MainWindow *mainWindow = new MainWindow(&settings);
    mainWindow->show();

    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");

    return app->exec();
}
