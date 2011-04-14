#include "mainwindow.h"

#include "settingsdialog.h"
#include "xbmc/xbmc.h"
#include "xbmc/xbmcconnection.h"
#include "xbmc/audioplayer.h"
#include "xbmc/audioplaylist.h"
#include "xbmc/videoplayer.h"
#include "xbmc/audiolibrary.h"

#include "qmlapplicationviewer.h"

#include <QAction>
#include <QMenuBar>
#include <QDebug>
#include <QDeclarativeContext>
#include <QApplication>
#include <QFileInfo>
#include <QLayout>
#include <QDesktopWidget>
#include <QSettings>

MainWindow::MainWindow()
{

    QAction *settingsAction = new QAction("&Settings", this);

    // Menus & Menu Bars
    QMenuBar *m_menuBar = new QMenuBar(this);

    QMenu *fileMenu = new QMenu(tr("&File"));
    m_menuBar->addMenu(fileMenu);
    fileMenu->addAction(settingsAction);

    connect(settingsAction, SIGNAL(triggered(bool)), this, SLOT(openSettings()));

    setMenuBar(m_menuBar);

    m_viewer = new QDeclarativeView;

    //    m_viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);

    QSettings settings;
    if(!settings.contains("Hostname")) {
        openSettings();
    }

    qDebug() << "connecting xbmc";
    Xbmc::XbmcConnection::connect(settings.value("Hostname").toString(), 9090);

    m_viewer->rootContext()->setContextProperty("MainWindow", this);

    xbmc = new Xbmc::Xbmc(this);
    m_viewer->rootContext()->setContextProperty("Xbmc", xbmc);

    m_viewer->rootContext()->setContextProperty("AudioPlayer", xbmc->audioPlayer());
    m_viewer->rootContext()->setContextProperty("AudioPlaylist", xbmc->audioPlayer()->playlist());
    m_viewer->rootContext()->setContextProperty("AudioLibrary", xbmc->audioLibrary());

    m_viewer->rootContext()->setContextProperty("VideoPlayer", xbmc->videoPlayer());

    setMainQmlFile("qml/xbmcremote/main.qml");
    //    viewer.showExpanded();

    m_viewer->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    setCentralWidget(m_viewer);

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true);
#endif


}


void MainWindow::openSettings()
{
    QSettings settings;
    SettingsDialog settingsDialog;
    settingsDialog.setHostname(settings.value("Hostname").toString());
    settingsDialog.exec();
    settings.setValue("Hostname", settingsDialog.hostname());
    Xbmc::XbmcConnection::connect(settingsDialog.hostname(), 9090);
}

void MainWindow::setMainQmlFile(const QString &file)
{
    QString adjustedFile = adjustPath(file);
    m_viewer->setSource(QUrl::fromLocalFile(adjustedFile));
}

QString MainWindow::adjustPath(const QString &path)
{
    const QString pathInShareDir = QCoreApplication::applicationDirPath()
            + QLatin1String("/../share/")
            + QFileInfo(QCoreApplication::applicationFilePath()).fileName()
            + QLatin1Char('/') + path;
    if (QFileInfo(pathInShareDir).exists())
        return pathInShareDir;
    return path;
}

QString MainWindow::state() const
{
    //    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    //    if (screenGeometry.width() > screenGeometry.height()) {
    //        qDebug() << "In Landscape Mode";
    //        return "landscape";
    //    } else {
    //        qDebug() << "In Portrait Mode";
    //        return "prtrait";
    //    }
    if(size().width() > size().height()) {
        return "landscape";
    } else {
        return "portrait";
    }
}

void MainWindow::myslot()
{
    qDebug() << "wff";
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    emit orientationChanged();
}
