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

#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QString>
#include <QVariantMap>

class PlaylistItem
{
public:
    PlaylistItem(const QString &file = QString(), const QString &playlist = QString());
    virtual ~PlaylistItem();

    void setFile(const QString &file);
    void setPlayList(const QString &playlist);

    QString file() const;
    QString playlistId() const;

    virtual QVariantMap toMap() const;

protected:
    QString m_file;
    QString m_playlistId;
};

#endif // PLAYLISTITEM_H
