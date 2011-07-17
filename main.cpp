#include "xbmc/xbmc.h"
#include "qmlapplicationviewer.h"
#include "settingsdialog.h"

#ifdef Q_WS_MAEMO_6
#include "meegohelper.h"
#endif

#include <QtGui/QApplication>
#include <QtDeclarative>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QmlApplicationViewer *viewer;
    QDeclarativeView *view;

#ifndef Q_WS_MAEMO_6

    QMainWindow *mainWindow = new QMainWindow();
#ifdef Q_WS_MAEMO_5
    mainWindow->setAttribute(Qt::WA_Maemo5AutoOrientation, true);
#endif
    viewer = new QmlApplicationViewer;
    mainWindow->setCentralWidget(viewer);

    SettingsDialog *settings = new SettingsDialog();

    QMenuBar *menuBar = new QMenuBar();
    QMenu *menu = menuBar->addMenu("File");
    menu->addAction("Settings", settings, SLOT(exec()));
    QAction *quitAction = menu->addAction("Quit xbmc", Xbmc::instance(), SLOT(quit()));
    Xbmc::instance()->connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), quitAction, SLOT(setEnabled(bool)));
    mainWindow->setMenuBar(menuBar);

    viewer->rootContext()->setContextProperty("xbmc", Xbmc::instance());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/xbmcremote/fremantle/main.qml"));
    mainWindow->show();
#else
    view = new QDeclarativeView;
    view->rootContext()->setContextProperty("xbmc", Xbmc::instance());
    view->setSource(QUrl("/opt/xbmcremote/qml/xbmcremote/harmattan/main.qml"));
    view->showFullScreen();
    MeeGoHelper *helper = new MeeGoHelper(&app);
#endif
    return app.exec();
}
