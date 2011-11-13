#include "networkauthhandler.h"

#include "xbmc/xbmc.h"

#include <QDebug>

NetworkAuthHandler::NetworkAuthHandler(QObject *parent) :
    QObject(parent)
{
}

void NetworkAuthHandler::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    qDebug() << "***************auth required!";
    Q_UNUSED(reply)
    if(!Xbmc::instance()->username().isEmpty() && !Xbmc::instance()->password().isEmpty()) {
        authenticator->setUser(Xbmc::instance()->username());
        authenticator->setPassword(Xbmc::instance()->password());
    }
}
