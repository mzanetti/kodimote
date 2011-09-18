#include "networkaccessmanagerfactory.h"
#include "xbmc/xbmcconnection.h"

QNetworkAccessManager* NetworkAccessManagerFactory::create(QObject *parent)
{
    return XbmcConnection::nam();
}
