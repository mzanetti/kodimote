#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <Plasma/Service>

class PlayerService: public Plasma::Service
{
  Q_OBJECT

public:
  PlayerService(QObject *parent = 0);


      Plasma::ServiceJob* createJob(const QString& operation,
                                  QMap<QString,QVariant>& parameters);

};


#endif