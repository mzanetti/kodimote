#ifndef NFCHANDLER_H
#define NFCHANDLER_H

#include <QObject>

#include <QNdefMessage>
#include <QNearFieldTarget>
#include <QNdefNfcTextRecord>
#include <QNdefNfcUriRecord>
QTM_USE_NAMESPACE

class NfcHandler : public QObject
{
    Q_OBJECT
public:
    explicit NfcHandler(QObject *parent = 0);


signals:
    void tagWritten();

public slots:
     void tagDetected(QNearFieldTarget* tag);
     void writeTag();
     void cancelWriteTag();

private slots:
     void requestCompleted(const QNearFieldTarget::RequestId &id);
     void ndefMessageRead(QNdefMessage);
     void ndefMessageWritten();

private:
     QList<QNearFieldTarget *> m_tagList;
     bool m_writeNextTag;

};

#endif // NFCHANDLER_H
