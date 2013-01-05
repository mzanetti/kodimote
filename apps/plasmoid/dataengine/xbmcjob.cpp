#include "xbmcjob.h"
#include "libxbmcremote/xbmc.h"

#include <QDebug>

XbmcJob::XbmcJob(const QString& operation,
                    QMap<QString,QVariant>& parameters,
                    QObject* parent):
  Plasma::ServiceJob(operation, operation, parameters, parent)
{


}


void XbmcJob::start()
{
  qDebug() << "should start job:" << operationName();
  
    if(operationName() == "SetVolume") {
      Xbmc::instance()->setVolume(parameters().value("level").toInt());
      connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectedChanged(bool)));
    }
}
