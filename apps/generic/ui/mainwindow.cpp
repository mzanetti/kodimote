/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
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

#include "mainwindow.h"
#include "settingsdialog.h"
#include "connectiondialog.h"
#include "aboutdialog.h"
#include "authenticationdialog.h"

#include "../qmlapplicationviewer/qmlapplicationviewer.h"

#include "libkodimote/kodi.h"
#include "libkodimote/videoplayer.h"
#include "libkodimote/audioplayer.h"
#include "libkodimote/kodidownload.h"
#include "libkodimote/settings.h"
#include "libkodimote/networkaccessmanagerfactory.h"

#include <QMenuBar>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QProcess>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>

#ifdef Q_WS_MAEMO_5

#include <QtGui/QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtContacts/QContactDetailFilter>
#include <QtContacts/QContactPhoneNumber>
#include <QtContacts/QContactManager>
QTM_USE_NAMESPACE
#endif

MainWindow::MainWindow(Settings *settings, QWidget *parent) :
    QMainWindow(parent),
    m_settings(settings),
    m_videoPaused(false),
    m_audioPaused(false)
{

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true);

    QDBusConnection::systemBus().connect(QString(), "/com/nokia/csd/call", "com.nokia.csd.Call", "Coming", this, SLOT(callEvent(QDBusObjectPath,QString)));
    QDBusConnection::systemBus().connect(QString(), "/com/nokia/csd/call", "com.nokia.csd.Call", "Created", this, SLOT(callEvent(QDBusObjectPath,QString)));

    grabZoomKeys(true);

//    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(orientationChanged()));

    connect(Kodi::instance(), SIGNAL(downloadAdded(KodiDownload*)), SLOT(downloadAdded(KodiDownload*)));
#endif

    viewer = new QmlApplicationViewer;
    setCentralWidget(viewer);


    QMenuBar *menuBar = new QMenuBar();
    QMenu *menu = menuBar->addMenu("Kodi");
    menu->addAction(tr("Connect..."), this, SLOT(openConnectionDialog()));
    menu->addAction(SettingsDialog::tr("Settings"), this, SLOT(openSettingsDialog()));

    QAction *quitAction = menu->addAction("Quit Kodi", Kodi::instance(), SLOT(quit()));
    Kodi::instance()->connect(Kodi::instance(), SIGNAL(connectedChanged(bool)), quitAction, SLOT(setEnabled(bool)));

    QMenu *helpMenu = menuBar->addMenu("Help");
    helpMenu->addAction(tr("About"), this, SLOT(openAboutDialog()));

    setMenuBar(menuBar);

    viewer->rootContext()->setContextProperty("settings", m_settings);
    viewer->rootContext()->setContextProperty("kodi", Kodi::instance());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/main.qml"));
    viewer->engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory());

    connect(Kodi::instance(), SIGNAL(authenticationRequired(QString,QString)), SLOT(authenticationRequired(QString,QString)), Qt::QueuedConnection);

    if(!Kodi::instance()->connecting()) {
        openConnectionDialog();
    }
}

MainWindow::~MainWindow()
{
#ifdef Q_WS_MAEMO_5
    grabZoomKeys(false);
#endif
}

void MainWindow::orientationChanged(){
#ifdef Q_WS_MAEMO_5
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    if (screenGeometry.width() > screenGeometry.height()) {
        qDebug() << "In Landscape Mode";
        showFullScreen();
    } else {
        qDebug() << "In Portrait Mode";
        showNormal();
    }
//    if(m_itemDialog) {
//        m_itemDialog->rotate(m_orientation);
//    }
#endif
}

void MainWindow::openSettingsDialog()
{
    SettingsDialog *settings = new SettingsDialog();
    settings->exec();
}

void MainWindow::openConnectionDialog()
{
    ConnectionDialog *connection = new ConnectionDialog();
    connection->exec();
}

void MainWindow::openAboutDialog()
{
    AboutDialog *about = new AboutDialog();
    about->exec();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F7:
        Kodi::instance()->volumeUp();
        event->accept();
        break;

    case Qt::Key_F8:
        Kodi::instance()->volumeDown();
        event->accept();
        break;
    }
    QWidget::keyPressEvent(event);
}

#ifdef Q_WS_MAEMO_5
void MainWindow::grabZoomKeys(bool grab) {
    if (!winId()) {
        qWarning("Can't grab keys unless we have a window id");
        return;
    }

    unsigned long val = (grab) ? 1 : 0;
    Atom atom = XInternAtom(QX11Info::display(), "_HILDON_ZOOM_KEY_ATOM", False);
    if (!atom) {
        qWarning("Unable to obtain _HILDON_ZOOM_KEY_ATOM. This example will only work "
                 "on a Maemo 5 device!");
        return;
    }

    XChangeProperty (QX11Info::display(),
             winId(),
             atom,
             XA_INTEGER,
             32,
             PropModeReplace,
             reinterpret_cast<unsigned char *>(&val),
             1);
}

#ifdef Q_WS_MAEMO_5
void MainWindow::callEvent(const QDBusObjectPath &param1, const QString &param2)
{
    qDebug() << "phone call event" << param1.path() << param2;
    Settings settings;

    if(settings.showCallNotifications()) {

        QDBusMessage msg = QDBusMessage::createMethodCall("com.nokia.csd", param1.path(), "com.nokia.csd.Call.Instance", "GetStatus");
        QDBusMessage reply = QDBusConnection::systemBus().call(msg);
        qDebug() << "call status:" << reply.arguments();

        if(reply.arguments().first().toInt() == 3) {

            QContactDetailFilter phoneFilter;
            phoneFilter.setDetailDefinitionName(QContactPhoneNumber::DefinitionName, QContactPhoneNumber::FieldNumber);
            phoneFilter.setValue(param2.right(6));
            phoneFilter.setMatchFlags(QContactFilter::MatchContains);
            qDebug() << "search contact";
            QContactManager contactManager;

            QString caller;
            if(contactManager.contacts(phoneFilter).count() > 0) {
                caller = contactManager.contacts(phoneFilter).first().displayLabel();
            } else {
                caller = param2;
            }

            qDebug() << "got contact" << caller;
            Kodi::instance()->sendNotification(tr("Incoming call"), caller);
        }
    }

    if(settings.changeVolumeOnCall()) {
        Kodi::instance()->dimVolumeTo(settings.volumeOnCall());
        m_videoPaused = true;
    }

    if(settings.pauseVideoOnCall() && Kodi::instance()->videoPlayer()->state() == "playing") {
        Kodi::instance()->videoPlayer()->playPause();
        m_audioPaused = true;
    }

    if(settings.pauseMusicOnCall() && Kodi::instance()->audioPlayer()->state() == "playing") {
        Kodi::instance()->audioPlayer()->playPause();
    }

    QDBusConnection::systemBus().connect(QString(), param1.path(), "com.nokia.csd.Call.Instance", "Terminated", this, SLOT(callTerminated()));

}
#endif

void MainWindow::callTerminated()
{
    Settings settings;
    if(settings.changeVolumeOnCall()) {
        Kodi::instance()->restoreVolume();
    }

    if(m_videoPaused && Kodi::instance()->videoPlayer()->state() != "playing") {
        Kodi::instance()->videoPlayer()->playPause();
    }
    if(m_audioPaused && Kodi::instance()->audioPlayer()->state() != "playing") {
        Kodi::instance()->audioPlayer()->playPause();
    }
}

void MainWindow::downloadAdded(KodiDownload *download)
{
    qDebug() << "Download added";
    connect(download, SIGNAL(finished(bool)), SLOT(downloadDone(bool)));
    connect(download, SIGNAL(started()), SLOT(downloadStarted()));
}

void MainWindow::downloadStarted()
{
    setAttribute(Qt::WA_Maemo5ShowProgressIndicator, true);

    KodiDownload *download = qobject_cast<KodiDownload*>(sender());

    QLabel *label = new QLabel(tr("Download started: %1").arg(download->label()));
    m_infoBox.setWidget(label);
    m_infoBox.show();
}

void MainWindow::downloadDone(bool success)
{
    setAttribute(Qt::WA_Maemo5ShowProgressIndicator, false);

    KodiDownload *download = qobject_cast<KodiDownload*>(sender());

    QLabel *label = new QLabel();
    if(success) {
        label->setText(tr("Download finished: %1").arg(download->label()));
    } else {
        label->setText(tr("Error downloading %1").arg("\"" + download->label() + "\""));
    }
    m_infoBox.setWidget(label);
    m_infoBox.show();

}

#endif

void MainWindow::authenticationRequired(const QString &hostname, const QString &address)
{
    Q_UNUSED(address);
    AuthenticationDialog *dialog = new AuthenticationDialog(hostname, this);
    dialog->exec();
}
