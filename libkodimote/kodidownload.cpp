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

#include "kodidownload.h"

KodiDownload::KodiDownload(QObject *parent) :
    QObject(parent),
    m_cancelled(false)
{
}

void KodiDownload::setSource(const QString &source)
{
    m_source = source;
    emit sourceChanged();
}

QString KodiDownload::source() const
{
    return m_source;
}

void KodiDownload::setDestination(const QString &destination)
{
    QString dest(destination);
    m_destination = dest.replace(':', ' ');
    emit destinationChanged();
}

QString KodiDownload::destination() const
{
    return m_destination;
}

void KodiDownload::setFile(QFile *file)
{
    m_file = file;
}

QFile *KodiDownload::file() const
{
    return m_file;
}

void KodiDownload::setTotal(qint64 total)
{
    m_total = total;
    emit totalChanged();
}

qint64 KodiDownload::total() const
{
    return m_total;
}

void KodiDownload::setProgress(qint64 progress)
{
    m_progress = progress;
    emit progressChanged();
}

qint64 KodiDownload::progress() const
{
    return m_progress;
}

void KodiDownload::setStarted()
{
    emit started();
}

void KodiDownload::setFinished(bool success)
{
    emit finished(success);
}

void KodiDownload::setIconId(const QString &iconId)
{
    m_icon = iconId;
}

QString KodiDownload::iconId() const
{
    return m_icon;
}

void KodiDownload::setLabel(const QString &label)
{
    m_label = label;
}

QString KodiDownload::label() const
{
    return m_label;
}

bool KodiDownload::isCancelled() const
{
    return m_cancelled;
}

void KodiDownload::cancel()
{
    emit cancelled();
    m_cancelled = true;
}
