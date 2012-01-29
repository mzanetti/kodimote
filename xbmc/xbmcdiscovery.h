#ifndef XBMCDISCOVERY_H
#define XBMCDISCOVERY_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class XbmcHost;

class XbmcDiscovery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool continuousDiscovery READ continuousDiscovery WRITE setContinuousDiscovery)

public:
    explicit XbmcDiscovery(QObject *parent = 0);
    ~XbmcDiscovery();

    bool continuousDiscovery();
    void setContinuousDiscovery(bool cd);

public slots:
    void discover();

private slots:
    void readDatagram();

private:
    bool setMulticastGroup(const QHostAddress &groupAddress, bool join);

    QTimer m_continuousDiscoveryTimer;
    QUdpSocket *m_socket;
    QHostAddress m_multicastAddress;
};

#endif // XBMCDISCOVERY_H
