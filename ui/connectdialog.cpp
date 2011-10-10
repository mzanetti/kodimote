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

#include "connectdialog.h"

#include <QGridLayout>
#include <QDialogButtonBox>
#include <QStandardItemModel>
#include <QLabel>
#include <QIntValidator>

#include "xbmc/xbmc.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent)
{

    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowTitle("XbmcRemote - " + tr("Connect to Xbmc"));

    QHBoxLayout *hLayout = new QHBoxLayout();
    QGridLayout *gridLayout = new QGridLayout();
    hLayout->addLayout(gridLayout);

#ifdef Q_WS_MAEMO_5
    setLayout(hLayout);
#else
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
#endif

    gridLayout->addWidget(new QLabel("Host:"), 0, 0);

    m_hostName = new QLineEdit(Xbmc::instance()->hostname());
    gridLayout->addWidget(m_hostName, 0, 1);

    gridLayout->addWidget(new QLabel("Http Port:"), 1, 0);

    m_port = new QLineEdit(QString::number(Xbmc::instance()->port()));
    m_port->setValidator(new QIntValidator());
    gridLayout->addWidget(m_port, 1, 1);

    gridLayout->addWidget(new QLabel("Username:"), 2, 0);

    m_userName = new QLineEdit(Xbmc::instance()->username());
    gridLayout->addWidget(m_userName, 2, 1);

    gridLayout->addWidget(new QLabel("Password:"), 3, 0);

    m_password = new QLineEdit(Xbmc::instance()->password());
    gridLayout->addWidget(m_password, 3, 1);


#ifdef Q_WS_MAEMO_5
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
    hLayout->addWidget(buttonBox);
#else
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    vLayout->addWidget(buttonBox);
#endif


    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ConnectDialog::setHostname(const QString &hostname)
{
    m_hostName->setText(hostname);
}

QString ConnectDialog::hostname()
{
    return m_hostName->text();
}

void ConnectDialog::setPort(int port)
{
    m_port->setText(QString::number(port));
}

int ConnectDialog::port()
{
    return m_port->text().toInt();
}

void ConnectDialog::setUsername(const QString &username)
{
    m_userName->setText(username);
}

QString ConnectDialog::username()
{
    return m_userName->text();
}

void ConnectDialog::setPassword(const QString &password)
{
    m_password->setText(password);
}

QString ConnectDialog::password()
{
    return m_password->text();
}

void ConnectDialog::accept()
{
    Xbmc::instance()->setHostname(m_hostName->text());
    Xbmc::instance()->setPort(m_port->text().toInt());
    Xbmc::instance()->setUsername(m_userName->text());
    Xbmc::instance()->setPassword(m_password->text());
    Xbmc::instance()->connectToHost();
    QDialog::accept();
}
