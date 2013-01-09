#ifndef CHANNELGROUPS_H
#define CHANNELGROUPS_H

#include "xbmclibrary.h"

class ChannelGroups : public XbmcLibrary
{
    Q_OBJECT
public:
    explicit ChannelGroups(XbmcModel *parent = 0);
    
    QString title() const;
    void refresh();
    XbmcModel* enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);
signals:
    
private slots:
    void listReceived(const QVariantMap &rsp);
    
};

#endif // CHANNELGROUPSS_H
