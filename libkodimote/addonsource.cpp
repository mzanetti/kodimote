#include "addonsource.h"
#include "libraryitem.h"

AddonSource::AddonSource(const QString &title, const QString &mediaType, const QString &dir, KodiModel *parent) :
    Files(mediaType, dir, parent),
    m_title(title)
{
    m_sort = false;
}

QString AddonSource::title() const
{
    return m_title;
}

KodiModel *AddonSource::enterItem(int index)
{
    LibraryItem *item = static_cast<LibraryItem*>(m_list.at(index));
    if(item->fileType() == "directory") {
        return new AddonSource(item->title(), m_mediaType, item->fileName(), this);
    }
    qDebug() << "cannot enter item of type file";
    return 0;
}

QString AddonSource::parseTitle(const QString &title) const
{
    return QString(title).replace(QRegExp("\\[/?\\w\\]"), "");
}
