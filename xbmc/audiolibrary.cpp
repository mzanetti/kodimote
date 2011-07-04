#include "audiolibrary.h"
#include "artists.h"
#include "albums.h"
#include "songs.h"
#include "xbmcconnection.h"

AudioLibrary::AudioLibrary(QObject *parent) :
    XbmcModel(0)
{
    m_itemlist.append("Artists");
    m_itemlist.append("Albums");
    m_itemlist.append("Titles");

}

int AudioLibrary::rowCount(const QModelIndex &parent) const
{
    return m_itemlist.count();
}

QVariant AudioLibrary::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_itemlist.at(index.row());
    case Qt::UserRole+1:
        return "directory";
    }
}

XbmcModel *AudioLibrary::enterItem(int index)
{
    switch(index) {
    case 0:
        return new Artists(this);
    case 1:
        return new Albums(-1, this);
    case 2:
        return new Songs(-1, -1, this);
    }
}

void AudioLibrary::playItem(int index)
{
    qDebug() << "cannot play whole audio library";
}

QString AudioLibrary::title() const
{
    return "Audio Library";
}
