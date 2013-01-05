#include "playerservice.h"
#include "playerjob.h"

PlayerService::PlayerService(QObject *parent) :
  Plasma::Service(parent)
{
    setObjectName("Player" + QLatin1String(" controller"));
    setName("org.kde.xbmcremote.Player");
    setDestination("Player");    

}

Plasma::ServiceJob* PlayerService::createJob(const QString& operation, QMap<QString,QVariant>& parameters)
{
  return new PlayerJob(operation, parameters, this);
}
