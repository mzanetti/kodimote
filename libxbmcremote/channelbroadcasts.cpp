#include "channelbroadcasts.h"
#include <QDebug>

ChannelBroadcasts::ChannelBroadcasts(QObject *parent) :
    QAbstractListModel(parent)
{
}

void ChannelBroadcasts::clear()
{
    beginResetModel();
    m_list.clear();
    endResetModel();
}

int ChannelBroadcasts::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant ChannelBroadcasts::data(const QModelIndex &index, int role) const
{
    qDebug() << "data called" << index << m_list.count();
    switch(role) {
    case RoleTitle:
        return m_list.at(index.row()).m_title;
    case RoleStartTime:
        return m_list.at(index.row()).m_startTime;
    case RoleEndTime:
        return m_list.at(index.row()).m_endTime;
    case RoleHasTimer:
        return m_list.at(index.row()).m_hasTimer;
    case RoleIsActive:
        return m_list.at(index.row()).m_isActive;
    case RoleProgressPercentage:
        return m_list.at(index.row()).m_progressPercentage;
    }
    return QVariant();
}

QHash<int, QByteArray> ChannelBroadcasts::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleTitle, "title");
    roles.insert(RoleStartTime, "startTime");
    roles.insert(RoleEndTime, "endTime");
    roles.insert(RoleHasTimer, "hasTimer");
    roles.insert(RoleIsActive, "isActive");
    roles.insert(RoleProgressPercentage, "progressPercentage");
    return roles;
}

void ChannelBroadcasts::addBroadcast(Broadcast broadcast)
{
    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    m_list.append(broadcast);
    endInsertRows();
}
