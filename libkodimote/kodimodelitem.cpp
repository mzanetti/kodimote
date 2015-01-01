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

#include "kodimodelitem.h"
#include "kodimodel.h"
KodiModelItem::KodiModelItem(const QString &title, const QString &subTitle, QObject *parent) :
    QObject(parent),
    m_title(title),
    m_subTitle(subTitle),
    m_ignoreArticle(false)
{

}

KodiModelItem::KodiModelItem(QObject *parent):
    QObject(parent)
{

}

KodiModelItem::~KodiModelItem()
{
    qDebug() << "deleting model item" << m_title;
}

QVariant KodiModelItem::data(int role) const
{
    switch(role) {
    case KodiModel::RoleTitle:
        return m_title;
    case KodiModel::RoleSubtitle:
        return m_subTitle;
    case KodiModel::RoleSortingTitle:
        QString ret = m_title;
        if(m_ignoreArticle) {
            ret.remove("The ");
        }
        return ret;

    }
    return QVariant();
}

QString KodiModelItem::title() const
{
    return m_title;
}

void KodiModelItem::setTitle(const QString &title)
{
    m_title = title;
    emit titleChanged();
}

QString KodiModelItem::subtitle() const
{
    return m_subTitle;
}

void KodiModelItem::setSubtitle(const QString &subtitle)
{
    m_subTitle = subtitle;
    emit subtitleChanged();
}

bool KodiModelItem::ignoreArticle() const
{
    return m_ignoreArticle;
}

void KodiModelItem::setIgnoreArticle(bool ignoreArticle)
{
    m_ignoreArticle = ignoreArticle;
    emit ignoreArticleChanged();
}
