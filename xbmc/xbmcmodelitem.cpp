#include "xbmcmodelitem.h"
#include "xbmcmodel.h"
XbmcModelItem::XbmcModelItem(const QString &title, const QString &subTitle) :
    m_title(title),
    m_subTitle(subTitle),
    m_ignoreArticle(false)
{

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
