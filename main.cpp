
#include "mainwindow.h"
#include "xbmc/xbmc.h"

#ifdef Q_WS_MAEMO_6
#include "meegohelper.h"
#endif

#include <QtGui/QApplication>
#include <QtDeclarative>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


#ifdef Q_WS_MAEMO_6

    QDeclarativeView *view;
    view = new QDeclarativeView;
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
