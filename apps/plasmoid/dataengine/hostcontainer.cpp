#include "hostcontainer.h"

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xbmcdiscovery.h"
#include "libxbmcremote/xbmchostmodel.h"

HostContainer::HostContainer(QObject *parent): Plasma::DataContainer(parent)
{
    m_discovery = new XbmcDiscovery(this);
    refresh();
    //discovery->setContinuousDiscovery(true);
      
    connect(Xbmc::instance()->hostModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(hostFound()));
}

void HostContainer::hostFound()
{
    if(Xbmc::instance()->hostModel()->rowCount(QModelIndex()) >= 1) {
	QStringList hostList;
	
	for(int i = 0; i < Xbmc::instance()->hostModel()->rowCount(QModelIndex()); ++i) {
		QModelIndex index = Xbmc::instance()->hostModel()->index(i, 0);
		qDebug() << "setting new data";
		hostList << Xbmc::instance()->hostModel()->data(index, Qt::DisplayRole).toString();
	}
	
	
	setData("hostList", hostList);
      }
            
      setNeedsUpdate(true);
      checkForUpdate();
}

void HostContainer::refresh()
{
    m_discovery->discover();
}

//#include "hostcontainer.moc"
