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

#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QLineEdit>

class QStackedLayout;
class QGridLayout;
class QListView;
class QDialogButtonBox;

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);

    void setHostname(const QString &hostname);
    QString hostname();

    void setPort(int port);
    int port();

    void setUsername(const QString &username);
    QString username();

    void setPassword(const QString &password);
    QString password();

public slots:
    void accept();

private slots:
    void showManualLayout();
    void showHostList();
    void enableOkButton();
    void removeHost();

private:
    QStackedLayout *m_stackedLayout;
    QGridLayout *m_manualLayout;
    QListView *m_hostView;
    QDialogButtonBox *m_buttonBox;
    QPushButton *m_manualButton;
    QPushButton *m_removeButton;

    QLineEdit *m_hostName;
    QLineEdit *m_port;
    QLineEdit *m_mac;
};

#endif // CONNECTDIALOG_H
