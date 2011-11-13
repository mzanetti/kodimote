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
