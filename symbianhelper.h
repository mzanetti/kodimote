#ifndef SYMBIANHELPER_H
#define SYMBIANHELPER_H

#include <QObject>

#include <remconcoreapitargetobserver.h>    // link against RemConCoreApi.lib
#include <remconcoreapitarget.h>            // and
#include <remconinterfaceselector.h>        // RemConInterfaceBase.lib

#include <etel3rdparty.h>

class Settings;
class CCallMonitor;

class MCallCallBack
{
public:
    virtual void CallStatusChangedL(CTelephony::TCallStatus& aStatus, TInt aError)=0;
};

class SymbianHelper : public QObject, public MRemConCoreApiTargetObserver, public MCallCallBack
{
    Q_OBJECT
public:
    explicit SymbianHelper(Settings *settings, QObject *parent = 0);

    virtual void MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct);
    virtual void CallStatusChangedL(CTelephony::TCallStatus &aStatus, TInt aError);
signals:

private slots:
    void connectionChanged(bool connected);
    void hostRemoved();

    void showCallNotification();
private:
    Settings *m_settings;

    CRemConInterfaceSelector* iInterfaceSelector;
    CRemConCoreApiTarget*     iCoreTarget;
    CCallMonitor* iCallMonitor;

    bool m_videoPaused;
    bool m_musicPaused;
};



class CCallMonitor : public CActive
{
public:
    ~CCallMonitor()
    {
        Cancel();
        delete iTelephony;
    }

    static CCallMonitor* NewLC(MCallCallBack& aObserver)
    {
        CCallMonitor* self = new (ELeave) CCallMonitor(aObserver);
        CleanupStack::PushL(self);
        self->ConstructL();
        return self;
    }
    static CCallMonitor* NewL(MCallCallBack& aObserver)
    {
        CCallMonitor* self = CCallMonitor::NewLC(aObserver);
        CleanupStack::Pop(); // self;
        return self;
    }

private:
    CCallMonitor(MCallCallBack& aCallBack)
        :CActive(EPriorityStandard),iCallBack(aCallBack),iCurrentStatusPckg(iCurrentStatus)
    {
        CActiveScheduler::Add(this);
    }

    void ConstructL()
    {
        iTelephony = CTelephony::NewL();
        StartListening();
    }

protected:
    void DoCancel()
    {
        iTelephony->CancelAsync(CTelephony::EVoiceLineStatusChangeCancel);
    }

    void RunL()
    {
        iCallBack.CallStatusChangedL(iCurrentStatus.iStatus,iStatus.Int());
        if(iStatus != KErrCancel)
            StartListening();
    }

private:
    void CancelOperation(void)
    {
        Cancel();
    }

    void StartListening()
    {
        Cancel();
        iCurrentStatus.iStatus = CTelephony::EStatusUnknown;
        iTelephony->NotifyChange(iStatus,CTelephony::EVoiceLineStatusChange,iCurrentStatusPckg);
        SetActive();
    }
private:
    MCallCallBack&                iCallBack;
    TInt                          iState;
    CTelephony::TCallStatusV1     iCurrentStatus;
    CTelephony::TCallStatusV1Pckg iCurrentStatusPckg;
    CTelephony*                   iTelephony;
};


#endif // SYMBIANHELPER_H
