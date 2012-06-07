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

#include <QScrollArea>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QStandardItemModel>
#include <QLabel>
#include <QIntValidator>
#include <QCheckBox>
#include <QSlider>

#include "xbmc/xbmc.h"
#include "settings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{

    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowTitle("XbmcRemote - " + tr("Settings"));

    QHBoxLayout *hLayout = new QHBoxLayout();

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget *widget = new QWidget();
    widget->setMaximumWidth(scrollArea->width());

    hLayout->addWidget(scrollArea);

    QVBoxLayout *gridLayout = new QVBoxLayout();
    widget->setLayout(gridLayout);


#ifdef Q_WS_MAEMO_5
    setLayout(hLayout);
#else
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
#endif

    Settings settings;

    m_cbThumbnails = new QCheckBox(tr("Use Thumbnails"));
    gridLayout->addWidget(m_cbThumbnails, 0, 0);
    m_cbThumbnails->setChecked(settings.useThumbnails());

    m_cbIgnoreArticles = new QCheckBox(tr("Ignore articles for sorting"));
    gridLayout->addWidget(m_cbIgnoreArticles, 0, 0);
    m_cbIgnoreArticles->setChecked(settings.ignoreArticle());

    m_cbVolume = new QCheckBox(tr("Change volume during calls"));
    gridLayout->addWidget(m_cbVolume, 0, 0);
    m_cbVolume->setChecked(settings.changeVolumeOnCall());

    m_slVolume = new QSlider(Qt::Horizontal);
    m_slVolume->setMaximum(100);
    gridLayout->addWidget(m_slVolume, 1, 0);
    m_slVolume->setValue(settings.volumeOnCall());

    m_cbPauseVideo = new QCheckBox(tr("Pause video during calls"));
    gridLayout->addWidget(m_cbPauseVideo, 2, 0);
    m_cbPauseVideo->setChecked(settings.pauseVideoOnCall());

    m_cbPauseMusic = new QCheckBox(tr("Pause music during calls"));
    gridLayout->addWidget(m_cbPauseMusic, 2, 0);
    m_cbPauseMusic->setChecked(settings.pauseMusicOnCall());

    connect(m_cbVolume, SIGNAL(clicked(bool)), m_slVolume, SLOT(setEnabled(bool)));

    m_cbShowCallNotification = new QCheckBox(tr("Show call notifications"));
    gridLayout->addWidget(m_cbShowCallNotification, 2, 0);
    m_cbShowCallNotification->setChecked(settings.showCallNotifications());

#ifdef Q_WS_MAEMO_5
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
    hLayout->addWidget(buttonBox);
#else
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    vLayout->addWidget(buttonBox);
#endif

    scrollArea->setWidget(widget);
    scrollArea->setWidgetResizable(true);


    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void SettingsDialog::accept()
{
    Settings settings;
    settings.setUseThumbnails(m_cbThumbnails->isChecked());
    settings.setIgnoreArticle(m_cbIgnoreArticles->isChecked());
    settings.setChangeVolumeOnCall(m_cbVolume->isChecked());
    settings.setVolumeOnCall(m_slVolume->value());
    settings.setPauseVideoOnCall(m_cbPauseVideo->isChecked());
    settings.setPauseMusicOnCall(m_cbPauseMusic->isChecked());
    settings.setShowCallNotifications(m_cbShowCallNotification->isChecked());
    QDialog::accept();
}
