#ifndef NETWORKACCESSMANAGERFACTORY_H
#define NETWORKACCESSMANAGERFACTORY_H

#ifdef QT5_BUILD

#include <QQmlNetworkAccessManagerFactory>
class NetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory

#else

#include <QDeclarativeNetworkAccessManagerFactory>
class NetworkAccessManagerFactory : public QDeclarativeNetworkAccessManagerFactory

#endif

{
public:
    virtual QNetworkAccessManager *create(QObject *parent);
};

#endif // NETWORKACCESSMANAGERFACTORY_H
