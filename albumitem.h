#ifndef ALBUMTITEM_H
#define ALBUMTITEM_H

#include <QString>

namespace Xbmc
{

class AlbumItem
{
public:
    AlbumItem(const QString &label, int id = -1);

    void setId(int id);
    void setLabel(const QString &label);

    int id() const;
    QString label() const;

private:
    int m_id;
    QString m_label;

};

}
#endif // ALBUMTITEM_H
