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

#ifndef PLASMA_APPLET_XBMCREMOTE_H
#define PLASMA_APPLET_XBMCREMOTE_H

class QAction;
class KCModuleProxy;

#include <QTimeLine>

#include <kdeversion.h>

#include <plasma/framesvg.h>
#include <plasma/svg.h>

#include <Plasma/PopupApplet>
#include <Plasma/ToolTipManager>

namespace Plasma
{
    class Applet;
} // namespace Plasma

class DeclarativePopup;

class XbmcremoteApplet : public Plasma::PopupApplet
{
Q_OBJECT
public:

    XbmcremoteApplet(QObject * parent, const QVariantList & args);
    ~XbmcremoteApplet();
    /* reimp Plasma::Applet */
    void init();
    /* reimp Plasma::Applet */
    void paintInterface(QPainter *painter, const QStyleOptionGraphicsItem * option,
                                            const QRect & rect);
    /* reimp Plasma::Applet */
    //QSizeF sizeHint(const Qt::SizeHint which, const QSizeF& constraint) const;
    /* reimp Plasma::Applet */
    //Qt::Orientations expandingDirections() const;
    /* reimp Plasma::Applet */
    void constraintsEvent(Plasma::Constraints constraints);

private Q_SLOTS:
    void connectedChanged(bool connected);
    void activePlayerChanged();
    void playStatusChanged();

private:
    void paintStatusOverlay(QPainter* p, QRect & rect);

    bool m_panelContainment;
    Plasma::Svg* m_svg;
    DeclarativePopup * m_popup;

};

#endif
