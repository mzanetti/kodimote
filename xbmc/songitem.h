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

#ifndef SONGITEM_H
#define SONGITEM_H

#include <QString>
#include <QTime>

class SongItem
{
public:
    SongItem(int songid = -1, const QString &label = QString());

    void setLabel(const QString &label);
    void setSongId(int songId);
    void setFile(const QString &file);
    void setTitle(const QString &title);
    void setArtist(const QString &artist);
    void setAlbum(const QString &album);
    void setThumbnail(const QString &thumbnail);
    void setFanart(const QString &fanart);
    void setDuration(const QTime &duration);


    int songId() const;
    QString file() const;
    QString label() const;
    QString title() const;
    QString artist() const;
    QString album() const;
    QString thumbnail() const;
    QString fanart() const;
    QTime duration() const;

private:
    int m_songId;
    QString m_file;
    QString m_label;
    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_thumbnail;
    QString m_fanart;
    QTime m_duration;
};

#endif // SONGITEM_H
