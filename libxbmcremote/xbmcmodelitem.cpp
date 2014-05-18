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

#include "xbmcmodelitem.h"
#include "xbmcmodel.h"
XbmcModelItem::XbmcModelItem(const QString &title, const QString &subTitle, QObject *parent) :
    QObject(parent),
    m_title(title),
    m_subTitle(subTitle),
    m_ignoreArticle(false)
{

}

XbmcModelItem::XbmcModelItem(QObject *parent):
    QObject(parent)
{

}

XbmcModelItem::~XbmcModelItem()
{
    qDebug() << "deleting model item" << m_title;
}

QVariant XbmcModelItem::data(int role) const
{
    switch(role) {
    case XbmcModel::RoleTitle:
        return m_title;
    case XbmcModel::RoleSubtitle:
        return m_subTitle;
    case XbmcModel::RoleSortingTitle:
        QString ret = m_title;
        if(m_ignoreArticle) {
            ret.remove("The ");
        }
        return ret;

    }
    return QVariant();
}

QString XbmcModelItem::title() const
{
    return m_title;
}

void XbmcModelItem::setTitle(const QString &title)
{
    m_title = title;
    emit titleChanged();
}

QString XbmcModelItem::subtitle() const
{
    return m_subTitle;
}

void XbmcModelItem::setSubtitle(const QString &subtitle)
{
    m_subTitle = subtitle;
    emit subtitleChanged();
}

bool XbmcModelItem::ignoreArticle() const
{
    return m_ignoreArticle;
}

void XbmcModelItem::setIgnoreArticle(bool ignoreArticle)
{
    m_ignoreArticle = ignoreArticle;
    emit ignoreArticleChanged();
}
