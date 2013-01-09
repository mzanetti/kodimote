#ifndef CHANNELS_H
#define CHANNELS_H

#include "xbmclibrary.h"

class Channels : public XbmcLibrary
{
    Q_OBJECT
public:
    explicit Channels(int channelgroupid, XbmcModel *parent = 0);
    
    QString title() const;
    void refresh();
    XbmcModel* enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);
signals:
    
private slots:
    void listReceived(const QVariantMap &rsp);

private:
    int m_channelgroupid;
};

#endif // CHANNELS_H
