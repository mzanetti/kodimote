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

    Q_INVOKABLE void fetchItemDetails(int index);
    Q_INVOKABLE bool hasDetails() { return false; } // Disabled for now as there is nothing interesting

signals:
    
private slots:
    void listReceived(const QVariantMap &rsp);
    void detailsReceived(const QVariantMap &rsp);

private:
    int m_channelgroupid;

    QMap<int, int> m_detailsRequestMap;
};

#endif // CHANNELS_H
