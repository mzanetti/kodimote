#ifndef NETWORKAUTHHANDLER_H
#define NETWORKAUTHHANDLER_H

#include <QObject>
#include <QNetworkReply>
#include <QAuthenticator>

class NetworkAuthHandler : public QObject
{
    Q_OBJECT
public:
    explicit NetworkAuthHandler(QObject *parent = 0);

public slots:
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
};

#endif // NETWORKAUTHHANDLER_H
