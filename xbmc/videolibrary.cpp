#include "videolibrary.h"
#include "movies.h"
#include "tvshows.h"
#include "musicvideos.h"
#include "xbmcconnection.h"

VideoLibrary::VideoLibrary(XbmcModel *parent) :
    XbmcModel(parent)
{
    m_list.append("Movies");
    m_list.append("TV Shows");
    m_list.append("Music Videos");
}

int VideoLibrary::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant VideoLibrary::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_list.at(index.row());
    case Qt::UserRole+1:
        return "directory";
    }
    return QVariant();
}

XbmcModel *VideoLibrary::enterItem(int index)
{
    switch(index) {
    case 0:
        return new Movies(this);
    case 1:
        return new TvShows(this);
    case 2:
        return new MusicVideos(this);
    }
    return 0;
}

void VideoLibrary::playItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "cannot play whole video library";
}

QString VideoLibrary::title() const
{
    return "Video Library";
}
