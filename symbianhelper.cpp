#include "symbianhelper.h"
#include "xbmc/xbmc.h"
#include "xbmc/videoplayer.h"
#include "xbmc/audioplayer.h"
#include "settings.h"

SymbianHelper::SymbianHelper(Settings *settings, QObject *parent) :
    QObject(parent),
    m_videoPaused(false),
    m_musicPaused(false)
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

    QT_TRAP_THROWING(iInterfaceSelector = CRemConInterfaceSelector::NewL());
    QT_TRAP_THROWING(iCoreTarget = CRemConCoreApiTarget::NewL(*iInterfaceSelector, *this));
    iInterfaceSelector->OpenTargetL();

    iCallMonitor = CCallMonitor::NewL(*this);
}

void SymbianHelper::MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct)
{
    //TRequestStatus status;
    switch( aOperationId )
    {
    case ERemConCoreApiVolumeUp:
        Xbmc::instance()->setVolume(Xbmc::instance()->volume() + 5);
        break;
    case ERemConCoreApiVolumeDown:
        Xbmc::instance()->setVolume(Xbmc::instance()->volume() - 5);
        break;
    default:
        break;
    }
}

void SymbianHelper::CallStatusChangedL(CTelephony::TCallStatus &aStatus, TInt aError)
{
    qDebug() << "call status changed" << aStatus;

    Settings settings;

    switch(aStatus) {
    // Incoming/outgoing call
    case CTelephony::EStatusRinging:
    case CTelephony::EStatusDialling:
        if(settings.changeVolumeOnCall()) {
            Xbmc::instance()->dimVolumeTo(settings.volumeOnCall());
        }
        if(settings.pauseVideoOnCall() && Xbmc::instance()->videoPlayer()->state() == "playing") {
            Xbmc::instance()->videoPlayer()->playPause();
            m_videoPaused = true;
        }

        if(settings.pauseMusicOnCall() && Xbmc::instance()->audioPlayer()->state() == "playing") {
            Xbmc::instance()->audioPlayer()->playPause();
            m_musicPaused = true;
        }
        break;

    // Call ended
    case CTelephony::EStatusIdle:
        if(m_settings->changeVolumeOnCall()) {
            Xbmc::instance()->restoreVolume();
        }

        if(m_videoPaused) {
            Xbmc::instance()->videoPlayer()->playPause();
        }
        if(m_musicPaused) {
            Xbmc::instance()->audioPlayer()->playPause();
        }

        break;

    }
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
