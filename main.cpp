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


#include "ui/mainwindow.h"
#include "xbmc/xbmc.h"
#include "settings.h"
#include "networkaccessmanagerfactory.h"
#include "xbmc/xdebug.h"

#ifdef Q_WS_MAEMO_6
#include "meegohelper.h"
#include "nfchandler.h"

#include <QtConnectivity/QNearFieldManager>
#include <QtConnectivity/QNdefNfcTextRecord>
#include <QtConnectivity/QNdefNfcUriRecord>

#endif

#include "qmlapplicationviewer.h"
#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QScopedPointer>
#include <QtSystemInfo/QSystemInfo>
QTM_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QTranslator translator;
    QSystemInfo info;http://developer.qt.nokia.com/doc/qt-4.8/qmake-using.html

#if defined Q_WS_MAEMO_6
    QScopedPointer<QApplication> app(createApplication(argc, argv));
#else
    QApplication *app = new QApplication( argc, argv );
#endif


    if (!translator.load("xbmcremote_" + info.currentLanguage() + ".qm", QString::fromLatin1("/opt/xbmcremote/i18n/"))) {
        qDebug() << "Cannot load translation file" << "/opt/xbmcremote/xbmcremote_" + info.currentLanguage() + ".ts";
    }

    qDebug() << "language:" << info.currentLanguage();
    app->installTranslator(&translator);

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

#if defined Q_WS_MAEMO_6 || defined QT_SIMULATOR

    QScopedPointer<QmlApplicationViewer> view(QmlApplicationViewer::create());

    view->rootContext()->setContextProperty("settings", &settings);
    view->rootContext()->setContextProperty("xbmc", Xbmc::instance());
#ifdef QT_SIMULATOR
    view->setSource(QUrl("qml/xbmcremote/harmattan/main.qml"));
#else
    view->setSource(QUrl("/opt/xbmcremote/qml/xbmcremote/harmattan/main.qml"));
#endif
    view->engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory());
    view->showFullScreen();

#ifdef Q_WS_MAEMO_6
    MeeGoHelper *helper = new MeeGoHelper(&settings, app.data());
    Q_UNUSED(helper)

    QNearFieldManager manager;
    NfcHandler nfcHandler;
    view->rootContext()->setContextProperty("nfcHandler", &nfcHandler);

    manager.setTargetAccessModes(QNearFieldManager::NdefReadTargetAccess | QNearFieldManager::NdefWriteTargetAccess);
    manager.startTargetDetection();

    QObject::connect(&manager, SIGNAL(targetDetected(QNearFieldTarget*)), &nfcHandler, SLOT(tagDetected(QNearFieldTarget*)));

#endif

#else

    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

#endif

    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");

    return app->exec();
}
