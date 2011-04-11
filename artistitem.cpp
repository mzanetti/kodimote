#include "artistitem.h"

namespace Xbmc
{

ArtistItem::ArtistItem(const QString &label, int id):
    m_id(id),
    m_label(label)
{

}

int ArtistItem::id() const
{
    return m_id;
}

QString ArtistItem::label() const
{
    return m_label;
}

}
