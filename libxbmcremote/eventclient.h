#ifndef EVENTCLIENT_H
#define EVENTCLIENT_H

#include "xbmc/xbmcclient.h"

#include <QObject>
#include <QChar>
#include <QTimer>

class XbmcHost;

class EventClient : public QObject
{
    Q_OBJECT
public:
    explicit EventClient(QObject *parent = 0);
    
    void connectToHost(XbmcHost *host);
    void disconnectFromHost();

public slots:
    void sendKeypress(const QString &buttonName);
    void sendIrPress(const QString &buttonName);

private slots:
    void releaseButton();

private:
    CAddress m_xbmcHost;
    int m_socket;
    QTimer m_releaseTimer;
};

#endif // EVENTCLIENT_H
