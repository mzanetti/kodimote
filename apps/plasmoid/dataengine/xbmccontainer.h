#ifndef XBMCCONTAINER_H
#define XBMCCONTAINER_H

#include <Plasma/DataContainer>

class XbmcContainer: public Plasma::DataContainer
{
    Q_OBJECT
public:
  XbmcContainer(QObject *parent = 0);

  
private slots:
  
  void volumeChanged();
  void stateChanged();
  void connectedChanged(bool connected);
};

#endif