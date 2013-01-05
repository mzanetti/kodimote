#include "networkauthhandler.h"

#include "xbmc.h"
#include "xbmcconnection.h"
#include "xbmchostmodel.h"

#include <QDebug>

NetworkAuthHandler::NetworkAuthHandler(QObject *parent) :
    QObject(parent)
{
}

void NetworkAuthHandler::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    qDebug() << "***************auth required!";
    Q_UNUSED(reply)
    if(XbmcConnection::connectedHost() && !XbmcConnection::connectedHost()->username().isEmpty() && !XbmcConnection::connectedHost()->password().isEmpty()) {
        authenticator->setUser(XbmcConnection::connectedHost()->username());
        authenticator->setPassword(XbmcConnection::connectedHost()->password());
    }
}
