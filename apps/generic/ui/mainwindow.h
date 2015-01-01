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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#ifdef Q_WS_MAEMO_5
#include <QtDBus/QDBusObjectPath>
#include <QtMaemo5/QMaemo5InformationBox>
#endif
#include "libkodimote/kodiconnection.h"

class Settings;
class QmlApplicationViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(Settings *settings, QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);

private slots:
    void orientationChanged();
    void openSettingsDialog();
    void openConnectDialog();
    void openAboutDialog();

#ifdef Q_WS_MAEMO_5
    void grabZoomKeys(bool grab);
    void callEvent(const QDBusObjectPath &, const QString &);
    void callTerminated();
    void downloadAdded(KodiDownload* download);
    void downloadStarted();
    void downloadDone(bool success);
#endif

    void authenticationRequired(const QString &hostname, const QString &address);
    void connectionChanged(bool connected);
    void hostRemoved();


private:
    QmlApplicationViewer *viewer;
    Settings *m_settings;

    bool m_videoPaused;
    bool m_audioPaused;

#ifdef Q_WS_MAEMO_5
    QMaemo5InformationBox m_infoBox;
#endif

};

#endif // MAINWINDOW_H
