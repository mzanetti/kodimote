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

#ifndef TITLES_H
#define TITLES_H

#include "xbmclibrary.h"

#include <QStandardItem>

class Songs : public XbmcLibrary
{
    Q_OBJECT
public:
    explicit Songs(int artistid = -1, int albumid = -1, XbmcModel *parent = 0);

    XbmcModel* enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;
    Q_INVOKABLE void fetchItemDetails(int index);
    Q_INVOKABLE bool hasDetails() { return true; }

private slots:
    void responseReceived(int id, const QVariantMap &map);

private:
    enum Request {
        RequestList,
        RequestDetails
    };

    QMap<int, Request> m_requestList;
    QMap<int, int> m_detailsRequestMap;

    int m_artistId;
    int m_albumId;
};

#endif // TITLES_H
