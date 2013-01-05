#include "networkaccessmanagerfactory.h"
#include "xbmcconnection.h"
#include "networkauthhandler.h"

QNetworkAccessManager* NetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(parent);
    NetworkAuthHandler *authHandler = new NetworkAuthHandler(parent);
    QObject::connect(nam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), authHandler, SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));
    return nam;
}
