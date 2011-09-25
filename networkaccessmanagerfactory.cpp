#include "networkaccessmanagerfactory.h"
#include "xbmc/xbmcconnection.h"

QNetworkAccessManager* NetworkAccessManagerFactory::create(QObject *)
{
    return XbmcConnection::nam();
}
