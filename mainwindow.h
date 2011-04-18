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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmlapplicationviewer.h"

#include <QMainWindow>

namespace Xbmc
{
class Xbmc;
}

class MainWindow: public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(QString state READ state NOTIFY orientationChanged)

public:
    MainWindow();

    QString state() const;

signals:
    void orientationChanged();

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void openSettings();
    void myslot();

private:
    QDeclarativeView *m_viewer;
    Xbmc::Xbmc *xbmc;


    void setMainQmlFile(const QString &file);
    QString adjustPath(const QString &path);
};

#endif // MAINWINDOW_H
