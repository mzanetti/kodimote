#ifndef XBMCCONNECTION_H
#define XBMCCONNECTION_H

#include <QObject>
#include <QTcpSocket>

namespace Xbmc
{
namespace XbmcConnection
{

void connect(const QString &hostname, int port);

int sendCommand(const QString &command, const QVariant &params = QVariant());

QString vfsPath();

class Notifier: public QObject
{
    Q_OBJECT
public:
    friend class XbmcConnectionPrivate;
signals:
    void receivedAnnouncement(const QVariantMap &announcement);
    void responseReceived(int id, const QVariant &params);
};
Notifier *notifier();
}

}
#endif // XBMCCONNECTION_H
