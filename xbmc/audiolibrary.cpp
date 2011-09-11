#include "audiolibrary.h"
#include "artists.h"
#include "albums.h"
#include "songs.h"
#include "xbmcconnection.h"

AudioLibrary::AudioLibrary() :
    XbmcModel(0)
{
    m_list.append(new QStandardItem("Artists"));
    m_list.append(new QStandardItem("Albums"));
    m_list.append(new QStandardItem("Titles"));

}

int AudioLibrary::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant AudioLibrary::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_list.at(index.row())->data(Qt::DisplayRole);
    case Qt::UserRole+1:
        return "directory";
    }
    return QVariant();
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
    return 0;
}

void AudioLibrary::playItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "cannot play whole audio library";
}

void AudioLibrary::addToPlaylist(int index)
{
    Q_UNUSED(index)
}

QString AudioLibrary::title() const
{
    return "Audio Library";
}

void AudioLibrary::scanForContent()
{
    XbmcConnection::sendCommand("AudioLibrary.Scan");
}
