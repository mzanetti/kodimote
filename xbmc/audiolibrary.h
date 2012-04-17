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

#ifndef AUDIOLIBRARY_H
#define AUDIOLIBRARY_H

#include "xbmclibrary.h"

#include <QStandardItem>

class AudioLibrary : public XbmcLibrary
{
    Q_OBJECT
public:
    explicit AudioLibrary();

    XbmcModel *enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;

public slots:
    void scanForContent();
    void refresh();

private:
    enum Request {
        RequestArtists,
        RequestAlbums
    };
    QMap<int, Request> m_requestMap;

};

#endif // AUDIOLIBRARY_H
