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

#if defined Q_WS_MAEMO_6
#include "meegohelper.h"
#include <MDeclarativeCache>
#endif

#if defined Q_WS_MAEMO_6 || defined Q_WS_SIMULATOR

#include <QtSystemInfo/QSystemInfo>
QTM_USE_NAMESPACE

#include "nfchandler.h"
#include "rumbleeffect.h"
#include "gesturehelper.h"
#endif
#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QDeclarativeView>
#include <QScopedPointer>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#if defined Q_WS_MAEMO_6
    QScopedPointer<QApplication> app(MDeclarativeCache::qApplication(argc, argv));
#elif defined Q_WS_SIMULATOR
    QScopedPointer<QApplication> app(new QApplication(argc, argv));
#endif
    QString language;

    // Load language file
    QSystemInfo info;
    language = info.currentLanguage();

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

    QDeclarativeView view;
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.setAttribute(Qt::WA_AutoOrientation, true);

    view.rootContext()->setContextProperty("xbmc", Xbmc::instance());

    Settings settings;
    view.rootContext()->setContextProperty("settings", &settings);

#ifdef Q_WS_MAEMO_6
    MeeGoHelper *helper = new MeeGoHelper(&settings, app.data());
    Q_UNUSED(helper)
#endif

    NfcHandler nfcHandler;
    view.rootContext()->setContextProperty("nfcHandler", &nfcHandler);

    RumbleEffect rumble;
    view.rootContext()->setContextProperty("rumbleEffect", &rumble);

    qmlRegisterType<GestureHelper>("xbmcremote", 1, 0, "GestureHelper");
    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");

    view.engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory());

// Set the main QML file for all the QML based platforms
#ifdef QT_SIMULATOR
    view.setSource(QUrl("qml/main.qml"));
#elif defined Q_WS_MAEMO_6
    view->setSource(QUrl("/opt/xbmcremote/qml/main.qml"));
#endif

    view.showFullScreen();

    return app->exec();
}
