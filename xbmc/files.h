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

#ifndef FILES_H
#define FILES_H

#include "xbmclibrary.h"

class Files : public XbmcLibrary
{
    Q_OBJECT
public:
    explicit Files(const QString &mediaType, const QString &dir, XbmcModel *parent = 0);

    XbmcModel *enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;
    Q_INVOKABLE void download(int index, const QString &path);

public slots:
    void refresh();

private slots:
    void responseReceived(int id, const QVariantMap &rsp);

private:
    int m_requestId;
    QString m_mediaType;
    QString m_dir;
};

#endif // FILES_H
