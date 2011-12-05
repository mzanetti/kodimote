#ifndef XBMCDISCOVERY_H
#define XBMCDISCOVERY_H

#include <QObject>
#include <QUdpSocket>

class XbmcDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit XbmcDiscovery(QObject *parent = 0);
    ~XbmcDiscovery();

    void discover();

signals:

private slots:
    void readDatagram();

private:
    bool setMulticastGroup(const QHostAddress &groupAddress, bool join);

    QUdpSocket *m_socket;
    QHostAddress m_multicastAddress;
};

#endif // XBMCDISCOVERY_H
