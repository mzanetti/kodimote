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

#ifndef EPISODES_H
#define EPISODES_H

#include "xbmclibrary.h"

#include <QStandardItem>

class Episodes : public XbmcLibrary
{
    Q_OBJECT
public:
    // seasonstring is a workaround to pass the label of the season because there is no sane way to qery the label here
    explicit Episodes(int tvshowid = -1, int seasonid = -1, const QString &seasonString = QString(), XbmcModel *parent = 0);

    XbmcModel *enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;

    Q_INVOKABLE void fetchItemDetails(int index);
    Q_INVOKABLE bool hasDetails() { return true; }

private slots:
    void responseReceived(int id, const QVariantMap &rsp);

private:
    enum Request {
        RequestList,
        RequestDetails
    };

    QMap<int, Request> m_requestList;
    QMap<int, int> m_detailsRequestMap;
    int m_tvshowid;
    int m_seasonid;
    QString m_seasonString;
};

#endif // EPISODES_H
