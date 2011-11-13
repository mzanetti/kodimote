#ifndef NETWORKACCESSMANAGERFACTORY_H
#define NETWORKACCESSMANAGERFACTORY_H

#include <QDeclarativeNetworkAccessManagerFactory>

class NetworkAccessManagerFactory : public QDeclarativeNetworkAccessManagerFactory
{
public:
    virtual QNetworkAccessManager *create(QObject *parent);
};

#endif // NETWORKACCESSMANAGERFACTORY_H
