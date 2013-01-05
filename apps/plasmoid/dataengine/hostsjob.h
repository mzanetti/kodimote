#ifndef HOSTSJOB_H
#define HOSTSJOB_H

#include <Plasma/ServiceJob>

class HostContainer;

class HostsJob: public Plasma::ServiceJob
{
    Q_OBJECT
    
public:
  HostsJob(HostContainer *container, const QString& operation,
                    QMap<QString,QVariant>& parameters,
                    QObject* parent);
  
  
  void start();
  
private slots:
  void connectedChanged(bool connected);
  
private:
  HostContainer *m_container;
};

#endif