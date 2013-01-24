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

#ifndef XBMCLIBRARY_H
#define XBMCLIBRARY_H

#include "xbmcmodel.h"

class LibraryItem;
class XbmcDownload;

class XbmcLibrary : public XbmcModel
{
    Q_OBJECT
    Q_PROPERTY(bool allowSearch READ allowSearch NOTIFY allowSearchChanged)
public:
    XbmcLibrary(XbmcModel *parent = 0);
    ~XbmcLibrary();

    virtual QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE virtual XbmcModel *enterItem(int index) = 0;
    Q_INVOKABLE virtual XbmcModel *exit();

    Q_INVOKABLE virtual void playItem(int index) = 0;
    Q_INVOKABLE virtual void addToPlaylist(int index) = 0;

    Q_INVOKABLE QVariant get(int index, const QString &roleName);
    Q_INVOKABLE XbmcModelItem *getItem(int index);

    Q_INVOKABLE virtual void fetchItemDetails(int index) { Q_UNUSED(index) }
    Q_INVOKABLE virtual bool hasShortDetails() { return false; }
    Q_INVOKABLE virtual bool hasDetails() { return false; }

    Q_INVOKABLE virtual void download(int index, const QString &path);
    Q_INVOKABLE void setDeleteAfterDownload(bool deleteAfterDownload);
    Q_INVOKABLE bool deleteAfterDownload() const;

    virtual bool allowSearch() { return true; }

protected:
    void startDownload(int index, XbmcDownload *download);

private slots:
    void downloadReceived(const QVariantMap &rsp);

    void currentItemChanged();

signals:
    void allowSearchChanged();

private:
    QMap<int, XbmcDownload*> m_downloadMap;
    bool m_deleteAfterDownload;

};

#endif // XBMCLIBRARY_H
