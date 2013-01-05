#ifndef HOSTCONTAINER_H
#define HOSTCONTAINER_H

#include <Plasma/DataContainer>

class XbmcDiscovery;

class HostContainer: public Plasma::DataContainer
{
    Q_OBJECT
public:
  HostContainer(QObject *parent = 0);
  
  void refresh();
  
private slots:
  void hostFound();

private:
  XbmcDiscovery *m_discovery;
  
};

#endif