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

#ifndef PICTURES_H
#define PICTURES_H

#include "kodimodel.h"

#include <QStandardItem>

//class Pictures : public KodiModel
//{
//    Q_OBJECT
//public:
//    explicit Pictures(KodiModel *parent = 0);
//    int rowCount(const QModelIndex &parent) const;
//    QVariant data(const QModelIndex &index, int role) const;

//    KodiModel* enterItem(int index);
//    void playItem(int index);
//    void addToPlaylist(int index);

//    QString title() const;

//private slots:
//    void responseReceived(int id, const QVariantMap &rsp);

//private:
//    int m_requestId;
//    int m_artistId;
//    int m_albumId;
//};

#endif // PICTURES_H
