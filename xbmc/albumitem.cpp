#include "albumitem.h"

namespace Xbmc
{

AlbumItem::AlbumItem(const QString &label, int id):
    m_id(id),
    m_label(label)
{

}

int AlbumItem::id() const
{
    return m_id;
}

QString AlbumItem::label() const
{
    return m_label;
}

}
