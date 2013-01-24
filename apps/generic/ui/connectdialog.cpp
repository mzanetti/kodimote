/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
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
#include <QListView>
#include <QStackedLayout>
#include <QPushButton>

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xbmchostmodel.h"
#include "libxbmcremote/xbmcdiscovery.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent)
{

    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowTitle("XbmcRemote - " + tr("Connect to XBMC"));

    m_stackedLayout = new QStackedLayout();

    XbmcDiscovery *discovery = new XbmcDiscovery(this);
    discovery->setContinuousDiscovery(true);

    m_hostView = new QListView();
    m_hostView->setModel(Xbmc::instance()->hostModel());
    m_stackedLayout->addWidget(m_hostView);

    QLabel *infoLabel = new QLabel(tr("Searching for XBMC hosts.") + "\n"
      + tr("Please enable the following options in the Network settings of XBMC:") + "\n- "
      + tr("Allow control of XBMC via HTTP") + "\n- "
      + tr("Allow programs on other systems to control XBMC") + "\n- "
      + tr("Announce these services to other systems via Zeroconf") + "\n"
      + tr("If you don't use Zeroconf, add a host manually."));
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignCenter);
    m_stackedLayout->addWidget(infoLabel);

    m_manualLayout = new QGridLayout();
    QWidget *manualWidget = new QWidget();
    manualWidget->setLayout(m_manualLayout);
    m_stackedLayout->addWidget(manualWidget);
#ifdef Q_WS_MAEMO_5
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addLayout(m_stackedLayout);
    setLayout(hLayout);
#else
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addLayout(m_stackedLayout);
    setLayout(vLayout);
#endif

    m_manualLayout->addWidget(new QLabel(tr("Host:")), 0, 0);

    m_hostName = new QLineEdit();
    m_manualLayout->addWidget(m_hostName, 0, 1);

    m_manualLayout->addWidget(new QLabel(tr("HTTP Port:")), 1, 0);

    m_port = new QLineEdit();
    m_port->setValidator(new QIntValidator());
    m_port->setText("8080");
    m_manualLayout->addWidget(m_port, 1, 1);

    m_manualLayout->addWidget(new QLabel(tr("MAC Address (optional):")), 2, 0);

    m_mac = new QLineEdit();
    m_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    m_manualLayout->addWidget(m_mac, 2, 1);

//    gridLayout->addWidget(new QLabel("Username:"), 2, 0);

//    m_userName = new QLineEdit(Xbmc::instance()->username());
//    gridLayout->addWidget(m_userName, 2, 1);

//    gridLayout->addWidget(new QLabel("Password:"), 3, 0);

//    m_password = new QLineEdit(Xbmc::instance()->password());
//    gridLayout->addWidget(m_password, 3, 1);


#ifdef Q_WS_MAEMO_5
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
    hLayout->addWidget(m_buttonBox);
#else
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    vLayout->addWidget(m_buttonBox);
#endif
    m_buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Connect"));
    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    m_manualButton = m_buttonBox->addButton(tr("Add Host"), QDialogButtonBox::ActionRole);
    connect(m_manualButton, SIGNAL(clicked()), SLOT(showManualLayout()));

    m_removeButton = m_buttonBox->addButton(tr("Remove Host"), QDialogButtonBox::ResetRole);
    connect(m_removeButton, SIGNAL(clicked()), SLOT(removeHost()));
    m_removeButton->setEnabled(false);

    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    if(Xbmc::instance()->hostModel()->rowCount(QModelIndex()) > 0) {
        showHostList();
    } else {
        m_stackedLayout->setCurrentIndex(1);
    }
    connect(Xbmc::instance()->hostModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(showHostList()));
    connect(m_hostView, SIGNAL(clicked(QModelIndex)), SLOT(enableOkButton()));

#ifdef Q_WS_MAEMO_5
    setMinimumHeight(400);
#endif
}

void ConnectDialog::accept()
{
    if(m_stackedLayout->currentIndex() == 0) {
        Xbmc::instance()->hostModel()->wakeup(m_hostView->currentIndex().row());
        Xbmc::instance()->hostModel()->connectToHost(m_hostView->currentIndex().row());
    } else {
        XbmcHost host;
        host.setHostname(m_hostName->text());
        host.setAddress(m_hostName->text());
        host.setPort(m_port->text().toInt());
        host.setHwAddr(m_mac->text());
        int newHostIndex = Xbmc::instance()->hostModel()->insertOrUpdateHost(host);
        Xbmc::instance()->hostModel()->connectToHost(newHostIndex);
    }
    QDialog::accept();
}

void ConnectDialog::showManualLayout()
{
    if(m_stackedLayout->currentIndex() != 2) {
        m_stackedLayout->setCurrentIndex(2);
        m_manualButton->setText(tr("Back"));
    } else {
        if(Xbmc::instance()->hostModel()->rowCount(QModelIndex()) > 0) {
            m_stackedLayout->setCurrentIndex(0);
        } else {
            m_stackedLayout->setCurrentIndex(1);
        }
        m_manualButton->setText(tr("Add Host"));
    }
    enableOkButton();
}

void ConnectDialog::showHostList()
{
    m_stackedLayout->setCurrentIndex(0);
}

void ConnectDialog::enableOkButton()
{
    switch(m_stackedLayout->currentIndex()) {
    case 0:
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_hostView->currentIndex().isValid());
        m_removeButton->setVisible(true);
        m_removeButton->setEnabled(m_hostView->currentIndex().isValid());
        break;
    case 1:
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        m_removeButton->setVisible(false);
        break;
    case 2:
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        m_removeButton->setVisible(false);
        break;
    }
}

void ConnectDialog::removeHost()
{
    Xbmc::instance()->hostModel()->removeHost(m_hostView->currentIndex().row());
    enableOkButton();
    if(Xbmc::instance()->hostModel()->rowCount(QModelIndex()) == 0) {
        m_stackedLayout->setCurrentIndex(1);
    }
}
