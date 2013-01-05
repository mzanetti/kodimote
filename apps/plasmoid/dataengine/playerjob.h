#ifndef PLAYERJOB_H
#define PLAYERJOB_H

#include <Plasma/ServiceJob>

class PlayerJob: public Plasma::ServiceJob
{
  Q_OBJECT

public:
  PlayerJob(const QString& operation,
                    QMap<QString,QVariant>& parameters,
                    QObject* parent);
  
  void start();
};

#endif