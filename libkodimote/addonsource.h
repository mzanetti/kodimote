#ifndef ADDONSOURCE_H
#define ADDONSOURCE_H

#include "files.h"

class AddonSource : public Files
{
    Q_OBJECT
public:
    explicit AddonSource(const QString &title, const QString &mediaType, const QString &dir, KodiModel *parent = 0);

    QString title() const;
    KodiModel *enterItem(int index);

protected:
    QString parseTitle(const QString &title) const;
    bool filterFile(const QVariantMap &item) const;

private:
    QString m_title;
};

#endif // ADDONSOURCE_H
