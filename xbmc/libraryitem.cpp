#include "libraryitem.h"

namespace Xbmc
{

LibraryItem::LibraryItem(const QString &label, int id):
    m_id(id),
    m_label(label)
{
}

int LibraryItem::id() const
{
    return m_id;
}

QString LibraryItem::label() const
{
    return m_label;
}

}
