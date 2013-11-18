#ifndef CHANNELBROADCASTS_H
#define CHANNELBROADCASTS_H

#include <QAbstractListModel>
#include <QDateTime>

class Broadcast {
public:
    QString m_title;
    QDateTime m_startTime;
    QDateTime m_endTime;
    int m_progressPercentage;
    bool m_hasTimer;
    bool m_isActive;
};

class ChannelBroadcasts : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleTitle,
        RoleStartTime,
        RoleEndTime,
        RoleProgressPercentage,
        RoleHasTimer,
        RoleIsActive
    };

    explicit ChannelBroadcasts(QObject *parent = 0);

    void clear();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int,QByteArray> roleNames() const;
    void addBroadcast(Broadcast broadcast);

signals:

public slots:

private:
    QList<Broadcast> m_list;
};

#endif // CHANNELBROADCASTS_H
