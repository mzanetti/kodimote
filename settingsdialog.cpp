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

#include "settingsdialog.h"

#include <QGridLayout>
#include <QDialogButtonBox>
#include <QStandardItemModel>
#include <QLabel>
#include <QIntValidator>

#include "xbmc/xbmc.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{

    setWindowTitle("XbmcRemote - " + tr("Settings"));
    QHBoxLayout *hLayout = new QHBoxLayout();
    setLayout(hLayout);

//    QVBoxLayout *layout = new QVBoxLayout();
//    hLayout->addLayout(layout);

    hLayout->addWidget(new QLabel("Host:"));

    m_hostName = new QLineEdit(Xbmc::instance()->hostname());
    hLayout->addWidget(m_hostName);

    hLayout->addWidget(new QLabel("Http Port:"));

    m_port = new QLineEdit(QString::number(Xbmc::instance()->port()));
    m_port->setValidator(new QIntValidator());
    hLayout->addWidget(m_port);



    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
    hLayout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void SettingsDialog::setHostname(const QString &hostname)
{
    m_hostName->setText(hostname);
}

QString SettingsDialog::hostname()
{
    return m_hostName->text();
}

void SettingsDialog::setPort(int port)
{
    m_port->setText(QString::number(port));
}

int SettingsDialog::port()
{
    return m_port->text().toInt();
}

void SettingsDialog::accept()
{
    Xbmc::instance()->setHostname(m_hostName->text());
    Xbmc::instance()->setPort(m_port->text().toInt());
    Xbmc::instance()->connectToHost();
    QDialog::accept();
}
