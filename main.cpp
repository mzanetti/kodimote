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

#ifdef Q_WS_MAEMO_6
#include "meegohelper.h"
#endif

#include <QtGui/QApplication>
#include <QtDeclarative>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Settings settings;

#ifdef Q_WS_MAEMO_6

    QDeclarativeView *view;
    view = new QDeclarativeView;
    view->rootContext()->setContextProperty("settings", &settings);
    view->rootContext()->setContextProperty("xbmc", Xbmc::instance());
    view->setSource(QUrl("/opt/xbmcremote/qml/xbmcremote/harmattan/main.qml"));
    view->showFullScreen();
    MeeGoHelper *helper = new MeeGoHelper(&app);

#else

    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

#endif

    return app.exec();
}
