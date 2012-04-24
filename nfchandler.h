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
    void tagError(const QString &errorMessage);

    public slots:
    void tagDetected(QNearFieldTarget* tag);
    void writeTag();
    void cancelWriteTag();
    void ndefMessageRead(QNdefMessage);

    private slots:
    void requestCompleted(const QNearFieldTarget::RequestId &id);
    void ndefMessageWritten();
    void error(QNearFieldTarget::Error error, const QNearFieldTarget::RequestId & id);
    private:
    QList<QNearFieldTarget *> m_tagList;
    bool m_writeNextTag;
    bool m_writeError;
};

#endif // NFCHANDLER_H
