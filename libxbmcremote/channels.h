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

#ifndef CHANNELS_H
#define CHANNELS_H

#include "xbmclibrary.h"

class Channels : public XbmcLibrary
{
    Q_OBJECT
public:
    explicit Channels(int channelgroupid, XbmcModel *parent = 0);
    
    QString title() const;
    void refresh();
    XbmcModel* enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    virtual QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    ThumbnailFormat thumbnailFormat() const { return ThumbnailFormat43; }
    Q_INVOKABLE void fetchItemDetails(int index);
    Q_INVOKABLE bool hasDetails() { return true; }

signals:
    
private slots:
    void listReceived(const QVariantMap &rsp);
    void detailsReceived(const QVariantMap &rsp);

    void fetchBroadcasts(int channelId);
    void broadcastsReceived(const QVariantMap &rsp);

private:
    int m_channelgroupid;

    QMap<int, int> m_detailsRequestMap;
    QMap<int, int> m_broadcastRequestMap;
};

#endif // CHANNELS_H
