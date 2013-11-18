#include "channelitem.h"

ChannelItem::ChannelItem(const QString &title, const QString &subtitle) :
    LibraryItem(title, subtitle),
    m_channelBroadcasts(new ChannelBroadcasts(this)),
    m_progressPercentage(0)
{
    setType("channel");
}

ChannelBroadcasts *ChannelItem::channelBroadcasts() const
{
    return m_channelBroadcasts;
}

int ChannelItem::progressPercent() const
{
    return m_progressPercentage;
}

void ChannelItem::setProgressPercentage(int percentage)
{
    if (m_progressPercentage != percentage) {
        m_progressPercentage = percentage;
        emit progressPercentageChanged();
    }
}
