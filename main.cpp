#include "xbmc/xbmc.h"
#include "qmlapplicationviewer.h"
#include "settingsdialog.h"

#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QtSystemInfo/QSystemDeviceInfo>

QTM_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSystemDeviceInfo devInfo;

    qDebug() << devInfo.model();

    QmlApplicationViewer *viewer;
    QDeclarativeView *view;
    if(devInfo.model() != "N9") {
        QMainWindow *mainWindow = new QMainWindow();
#ifdef Q_WS_MAEMO_5
        mainWindow->setAttribute(Qt::WA_Maemo5AutoOrientation, true);
#endif
        viewer = new QmlApplicationViewer;
        mainWindow->setCentralWidget(viewer);

        SettingsDialog *settings = new SettingsDialog();

        QMenuBar *menuBar = new QMenuBar();
        menuBar->addMenu("File")->addAction("Settings", settings, SLOT(exec()));
        mainWindow->setMenuBar(menuBar);

        viewer->rootContext()->setContextProperty("xbmc", Xbmc::instance());
        viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
        viewer->setMainQmlFile(QLatin1String("qml/xbmcremote/fremantle/main.qml"));
        mainWindow->show();
    } else {
        view = new QDeclarativeView;
        view->rootContext()->setContextProperty("xbmc", Xbmc::instance());
        view->setSource(QUrl("/opt/xbmcremote/qml/xbmcremote/harmattan/main.qml"));
        view->showFullScreen();
    }
    return app.exec();
}
