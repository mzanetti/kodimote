#ifndef ARTISTITEM_H
#define ARTISTITEM_H

#include <QString>

namespace Xbmc
{

class ArtistItem
{
public:
    ArtistItem(const QString &label, int id = -1);

    void setId(int id);
    void setLabel(const QString &label);

    int id() const;
    QString label() const;

private:
    int m_id;
    QString m_label;

};

}
#endif // ARTISTITEM_H
