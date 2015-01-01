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

#include "authenticationdialog.h"
#include "libkodimote/kodi.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>

AuthenticationDialog::AuthenticationDialog(const QString &hostname, QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowTitle("Kodimote - " + tr("Authentication"));

#ifdef Q_WS_MAEMO_5
    QHBoxLayout *layout = new QHBoxLayout();

#else
    QVBoxLayout *layout = new QVBoxLayout();

#endif
    setLayout(layout);

    QVBoxLayout *vLayout = new QVBoxLayout();
    QLabel *mainLabel = new QLabel(tr("XBMC on %1 requires authentication:").arg(hostname));
    vLayout->addWidget(mainLabel);

    QGridLayout *gridLayout = new QGridLayout();
    vLayout->addLayout(gridLayout);

    QLabel *userLabel = new QLabel(tr("Username:"));
    gridLayout->addWidget(userLabel, 0, 0);

    m_userEdit = new QLineEdit();
    gridLayout->addWidget(m_userEdit, 0, 1);

    QLabel *passwordLabel = new QLabel(tr("Password:"));
    gridLayout->addWidget(passwordLabel, 1, 0);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    gridLayout->addWidget(m_passwordEdit, 1, 1);

    layout->addLayout(vLayout);

#ifdef Q_WS_MAEMO_5
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
#else
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
#endif
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    layout->addWidget(buttonBox);
}

void AuthenticationDialog::accept()
{
    Kodi::instance()->setAuthCredentials(m_userEdit->text(), m_passwordEdit->text());
    QDialog::accept();
}
