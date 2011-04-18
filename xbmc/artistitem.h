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

#ifndef ARTISTITEM_H
#define ARTISTITEM_H

#include <QString>

namespace Xbmc
{

class ArtistItem
{
public:
    ArtistItem(const QString &label, int id = -1);

    void setId(int id);
    void setLabel(const QString &label);

    int id() const;
    QString label() const;

private:
    int m_id;
    QString m_label;

};

}
#endif // ARTISTITEM_H
