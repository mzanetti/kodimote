#ifndef CHANNELITEM_H
#define CHANNELITEM_H

#include "libraryitem.h"
#include "channelbroadcasts.h"

class ChannelItem : public LibraryItem
{
    Q_OBJECT

    Q_PROPERTY(ChannelBroadcasts* channelBroadcasts READ channelBroadcasts NOTIFY channelBroadcastsChanged)
    Q_PROPERTY(int progressPercent READ progressPercent NOTIFY progressPercentageChanged)

public:
    explicit ChannelItem(const QString &title = QString(), const QString &subtitle = QString(), QObject *parent = 0);
    ~ChannelItem();

    ChannelBroadcasts* channelBroadcasts() const;

    int progressPercent() const;
    void setProgressPercentage(int percentage);

signals:
    void channelBroadcastsChanged();
    void progressPercentageChanged();

public slots:
private:
    ChannelBroadcasts *m_channelBroadcasts;
    int m_progressPercentage;

};

#endif // CHANNELITEM_H
