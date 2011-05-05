#include "videolibrary.h"

namespace Xbmc
{

VideoLibrary::VideoLibrary(Player *player, QObject *parent):
    QAbstractItemModel(parent),
    m_player(player)
{

}

void VideoLibrary::enterItem(int index)
{

}

void VideoLibrary::showLibrary()
{

}

void VideoLibrary::goUp(int levels)
{

}

void VideoLibrary::responseReceived(int, const QVariantMap &response)
{

}

QString VideoLibrary::state()
{
    return m_state;
}

QString VideoLibrary::currentDir()
{
    return "/";
}

QModelIndex VideoLibrary::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex VideoLibrary::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int VideoLibrary::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

int VideoLibrary::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant VideoLibrary::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

}
