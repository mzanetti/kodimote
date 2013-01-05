#include "xbmccontainer.h"

#include "libxbmcremote/xbmc.h"

XbmcContainer::XbmcContainer(QObject *parent): Plasma::DataContainer(parent)
{

  volumeChanged();
  connect(Xbmc::instance(), SIGNAL(volumeChanged(int)), SLOT(volumeChanged()));
  
  stateChanged();
  connect(Xbmc::instance(), SIGNAL(stateChanged()), SLOT(stateChanged()));
  
  connectedChanged(false);
  connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectedChanged(bool)));
}

void XbmcContainer::volumeChanged()
{
  qDebug() << "volume changed";
  setData("volume", Xbmc::instance()->volume());
  checkForUpdate();
}

void XbmcContainer::stateChanged()
{
    setData("state", Xbmc::instance()->state());
    checkForUpdate();
}

void XbmcContainer::connectedChanged(bool connected)
{
    setData("connected", connected);
    checkForUpdate();
}
