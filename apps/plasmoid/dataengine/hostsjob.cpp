#include "hostsjob.h"
#include "hostcontainer.h"

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xbmchostmodel.h"

#include <QDebug>


HostsJob::HostsJob(HostContainer *container, const QString& operation,
                    QMap<QString,QVariant>& parameters,
                    QObject* parent):
  Plasma::ServiceJob(operation, operation, parameters, parent),
  m_container(container)
{
    
}

void HostsJob::start()
{

  qDebug() << "Should start operation:" << operationName() << parameters();
  
  if(operationName() == "Discover") {
      m_container->refresh();
  } else if(operationName() == "Connect") {
      Xbmc::instance()->hostModel()->connectToHost(parameters().value("id").toInt());
      connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectedChanged(bool)));
  }
  
  
}


void HostsJob::connectedChanged(bool connected)
{
  disconnect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), this, SLOT(connectedChanged(bool)));
  setError(NoError);
  emitResult();
}
