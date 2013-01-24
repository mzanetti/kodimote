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

#include "aboutdialog.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QDesktopServices>
#include <QUrl>

#define VERSION_STRING "1.3.0"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowTitle(tr("About") + " Xbmcremote");

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setSpacing(20);

#ifdef Q_WS_MAEMO_5
    QPixmap icon = QPixmap(":/logo_l").scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
#else
    QPixmap icon = QPixmap(":/logo_l").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
#endif
    QLabel *iconLabel = new QLabel();
    iconLabel->setAlignment(Qt::AlignTop);
    iconLabel->setPixmap(icon);
    hlayout->addWidget(iconLabel);

    QVBoxLayout *vlayout = new QVBoxLayout;
    hlayout->addLayout(vlayout);

    QLabel *label = new QLabel(QString("Xbmcremote ") + VERSION_STRING);
    QFont font;
    font.setPixelSize(24);
    label->setFont(font);
    vlayout->addWidget(label);

    QLabel *label1 = new QLabel(tr("Copyright") + "\nMichael Zanetti <michael_zanetti@gmx.net>");
    vlayout->addWidget(label1);

    QLabel *label2 = new QLabel(tr("Thanks to") + "\n" \
                                "XBMC Development Team\n");
    vlayout->addWidget(label2);


    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *closeButton = new QPushButton(tr("Close"));
    buttonLayout->addWidget(closeButton);
    connect(closeButton, SIGNAL(clicked()), SLOT(close()));

    QPushButton *donateButton = new QPushButton(tr("Donate"));
    buttonLayout->addWidget(donateButton);
    connect(donateButton, SIGNAL(clicked()), SLOT(openDonationPage()));

    QPushButton *flattrButton = new QPushButton(tr("Flattr"));
    buttonLayout->addWidget(flattrButton);
    connect(flattrButton, SIGNAL(clicked()), SLOT(openFlattrPage()));

    vlayout->addLayout(buttonLayout);

    setLayout(hlayout);
}

void AboutDialog::openDonationPage()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CWFYRZH8XNYF2"));
}

void AboutDialog::openFlattrPage()
{
    QDesktopServices::openUrl(QUrl("http://flattr.com/thing/412274/Xbmcremote"));
}
