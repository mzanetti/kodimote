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
#include <QTime>

class PlaylistItem: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString durationString READ durationString NOTIFY durationChanged)
    Q_PROPERTY(QString fanart READ fanart NOTIFY fanartChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)

public:
    PlaylistItem(QObject *parent = 0);
    virtual ~PlaylistItem();

    // those will be filled in in toMap()
    void setFile(const QString &file);
    void setPlayList(const QString &playlist);

    QString file() const;
    QString playlistId() const;

    virtual QVariantMap toMap() const;

    void setLabel(const QString &label);
    void setDuration(const QTime &duration);
    void setTitle(const QString &title);
    void setFanart(const QString &fanart);
    void setThumbnail(const QString &thumbnail);

    QString label() const;
    QTime duration() const;
    QString durationString() const;
    QString title() const;
    QString fanart() const;
    QString thumbnail() const;

signals:
    void titleChanged();
    void durationChanged();
    void labelChanged();
    void fanartChanged();
    void thumbnailChanged();

protected:
    QString m_file;
    QString m_playlistId;

    QString m_label;
    QTime m_duration;
    QString m_title;
    QString m_fanart;
    QString m_thumbnail;
};

#endif // PLAYLISTITEM_H
