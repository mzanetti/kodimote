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

#ifndef ALBUMS_H
#define ALBUMS_H

#include "kodilibrary.h"

#include <QStandardItem>

class Songs;

class Albums : public KodiLibrary
{
    Q_OBJECT
public:
    explicit Albums(int artistId = -1, int genreId = -1, KodiModel *parent = 0);
    ~Albums();

    KodiModel* enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;

    Q_INVOKABLE void fetchItemDetails(int index);
    Q_INVOKABLE bool hasDetails() { return true; }

    Q_INVOKABLE virtual void download(int index, const QString &path);

public slots:
    void refresh();

private slots:
    void listReceived(const QVariantMap &rsp);
    void detailsReceived(const QVariantMap &rsp);

    void downloadModelFilled();

private:
    QMap<int, int> m_detailsRequestMap;

    int m_artistId;
    int m_genreId;
    QList<Songs*> m_downloadList;
    QString m_downloadPath;
};

#endif // ALBUMS_H
