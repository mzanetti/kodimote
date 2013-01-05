#ifndef XBMCJOB_H
#define XBMCJOB_H

#include <Plasma/ServiceJob>

class XbmcJob: public Plasma::ServiceJob
{
  Q_OBJECT

public:
  XbmcJob(const QString& operation,
                    QMap<QString,QVariant>& parameters,
                    QObject* parent);
  
  void start();
};

#endif