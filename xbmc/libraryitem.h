#ifndef LIBRARYITEM_H
#define LIBRARYITEM_H

#include <QString>

namespace Xbmc
{

class LibraryItem
{
public:
    LibraryItem(const QString &label, int id = -1);

    QString label() const;
    int id() const;

private:
    int m_id;
    QString m_label;
};

}

#endif // LIBRARYITEM_H
