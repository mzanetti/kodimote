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

#ifndef XBMCLIBRARY_H
#define XBMCLIBRARY_H

#include "kodimodel.h"

class LibraryItem;
class KodiDownload;

class KodiLibrary : public KodiModel
{
    Q_OBJECT
public:
    KodiLibrary(KodiModel *parent = 0);
    ~KodiLibrary();

    virtual QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE virtual KodiModel *enterItem(int index) = 0;
    Q_INVOKABLE virtual KodiModel *exit();

    Q_INVOKABLE virtual void playItem(int index, bool resume = false) = 0;
    Q_INVOKABLE virtual void addToPlaylist(int index) = 0;

    Q_INVOKABLE QVariant get(int index, const QString &roleName);
    Q_INVOKABLE KodiModelItem *getItem(int index);

    Q_INVOKABLE virtual void fetchItemDetails(int index) { Q_UNUSED(index) }
    Q_INVOKABLE virtual bool hasShortDetails() { return false; }
    Q_INVOKABLE virtual bool hasDetails() { return false; }

    Q_INVOKABLE virtual void download(int index, const QString &path);
    Q_INVOKABLE void setDeleteAfterDownload(bool deleteAfterDownload);
    Q_INVOKABLE bool deleteAfterDownload() const;

protected:
    void startDownload(int index, KodiDownload *download);

private slots:
    void downloadReceived(const QVariantMap &rsp);

    void currentItemChanged();

private:
    QMap<int, KodiDownload*> m_downloadMap;
    bool m_deleteAfterDownload;

};

#endif // XBMCLIBRARY_H
