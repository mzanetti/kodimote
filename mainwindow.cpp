#include "mainwindow.h"
#include "settingsdialog.h"
#include "aboutdialog.h"

#include "qmlapplicationviewer.h"

#include "xbmc/xbmc.h"

#include <QMenuBar>
#include <QDeclarativeContext>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true);
#endif

    viewer = new QmlApplicationViewer;
    setCentralWidget(viewer);


    QMenuBar *menuBar = new QMenuBar();
    QMenu *menu = menuBar->addMenu("Xbmc");
    menu->addAction("Connect...", this, SLOT(openSettingsDialog()));

    QAction *quitAction = menu->addAction("Quit xbmc", Xbmc::instance(), SLOT(quit()));
    Xbmc::instance()->connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), quitAction, SLOT(setEnabled(bool)));

    QMenu *helpMenu = menuBar->addMenu("Help");
    helpMenu->addAction("About", this, SLOT(openAboutDialog()));

    setMenuBar(menuBar);

    viewer->rootContext()->setContextProperty("xbmc", Xbmc::instance());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/xbmcremote/fremantle/main.qml"));

}

void MainWindow::openSettingsDialog()
{
    SettingsDialog *settings = new SettingsDialog();
    settings->exec();
}

void MainWindow::openAboutDialog()
{
    AboutDialog *about = new AboutDialog();
    about->exec();
}
