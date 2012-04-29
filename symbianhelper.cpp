#include "settings.h"
#include "symbianhelper.h"
#include "xbmc/xbmc.h"

SymbianHelper::SymbianHelper(Settings *settings, QObject *parent) :
    QObject(parent)
{
    // Load stored hosts
    foreach(const XbmcHost &host, settings->hostList()) {
        int index = Xbmc::instance()->hostModel()->insertOrUpdateHost(host);
        if(host.address() == settings->lastHost().address()) {
            qDebug() << "reconnecting to" << host.hostname() << host.address() << host.username() << host.password();
            Xbmc::instance()->hostModel()->connectToHost(index);
        }
    }

    connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectionChanged(bool)));
    connect(Xbmc::instance()->hostModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(hostRemoved()));

}

void SymbianHelper::connectionChanged(bool connected)
{
    if(connected) {
        m_settings->addHost(*Xbmc::instance()->connectedHost());
        m_settings->setLastHost(*Xbmc::instance()->connectedHost());
    }
}

void SymbianHelper::hostRemoved()
{
    // We need to check if all our stored hosts are still in hostList
    for(int i = 0; i < m_settings->hostList().count();) {
        bool found = false;
        for(int j = 0; j < Xbmc::instance()->hostModel()->rowCount(QModelIndex()); ++j) {
            if(m_settings->hostList().at(i).address() == Xbmc::instance()->hostModel()->get(j, "address").toString()) {
                found = true;
                break;
            }
        }
        if(!found) {
            m_settings->removeHost(m_settings->hostList().at(i));
            qDebug() << "removed host" << i;
        } else {
            ++i;
        }
    }
}
