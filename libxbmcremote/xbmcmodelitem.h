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

#ifndef XBMCMODELITEM_H
#define XBMCMODELITEM_H

#include <QObject>

class XbmcModelItem: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(bool ignoreArticle READ ignoreArticle WRITE setIgnoreArticle NOTIFY ignoreArticleChanged)

public:
    XbmcModelItem(const QString &title = QString(), const QString &subTitle = QString());
    ~XbmcModelItem() {}

    virtual QVariant data(int role) const;

    QString title() const;
    void setTitle(const QString & title);

    QString subtitle() const;
    void setSubtitle(const QString & subtitle);

    bool ignoreArticle() const;
    void setIgnoreArticle(bool ignoreArticle);

signals:
    void titleChanged();
    void subtitleChanged();
    void ignoreArticleChanged();

private:
    QString m_title;
    QString m_subTitle;
    bool m_ignoreArticle;
};


#endif // XBMCMODELITEM_H
