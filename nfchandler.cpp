#include "nfchandler.h"
#include "xbmc/xbmc.h"

#include <QDebug>
#include <QUrl>
#include <QVariant>
#include <QStringList>

NfcHandler::NfcHandler(QObject *parent) :
    QObject(parent),
    m_writeNextTag(false)
{
}


void NfcHandler::tagDetected(QNearFieldTarget *tag)
{
    if(!m_tagList.contains(tag)) {
        connect(tag, SIGNAL(requestCompleted(QNearFieldTarget::RequestId)), this, SLOT(requestCompleted(QNearFieldTarget::RequestId)));
        connect(tag, SIGNAL(ndefMessageRead(QNdefMessage)), this, SLOT(ndefMessageRead(QNdefMessage)));
        connect(tag, SIGNAL(ndefMessagesWritten()), SLOT(ndefMessageWritten()));
        m_tagList.append(tag);
    }

    if(m_writeNextTag) {
        QNdefNfcTextRecord record;
        record.setText("xbmc:" + Xbmc::instance()->hostname() + ':' + QString::number(Xbmc::instance()->port()));
        QNdefMessage message(record);
        qDebug() << "writing record:" << record.text();
        tag->writeNdefMessages(QList<QNdefMessage>() << message);
        m_writeNextTag = false;
        return;
    }

    tag->readNdefMessages();
}

void NfcHandler::requestCompleted(const QNearFieldTarget::RequestId &id)
{
    Q_UNUSED(id)
    qDebug() << "NFC request completed";
}

void NfcHandler::ndefMessageRead(QNdefMessage message)
{
    foreach (const QNdefRecord &record, message) {
        if (record.isRecordType<QNdefNfcTextRecord>()) {
            QNdefNfcTextRecord textRecord(record);
            if(textRecord.text().startsWith("xbmc:") && textRecord.text().split(':').count() == 3) {
                QString hostname = textRecord.text().split(':').at(1);
                QString port = textRecord.text().split(':').at(2);
                qDebug() << "Should connect to" << hostname << ':' << port;
                Xbmc::instance()->setHostname(hostname);
                Xbmc::instance()->setPort(port.toInt());
                Xbmc::instance()->connectToHost();
            } else {
                qDebug() << "NDEF text record not compatible with xbmcremote:" << textRecord.text();
            }
        } else if (record.isRecordType<QNdefNfcUriRecord>()) {
            QNdefNfcUriRecord uriRecord(record);
            qDebug() << "********** Got URI record:" << uriRecord.uri();
        }else if (record.typeNameFormat() == QNdefRecord::Mime &&
                  record.type().startsWith("image/")) {
            qDebug() << "got image...";
        }else if (record.typeNameFormat() == QNdefRecord::NfcRtd ) {
            qDebug() << "Got Rtd tag" << record.payload();
            QNdefNfcUriRecord uri(record);
            qDebug() << "uri:" << uri.uri();
        }else if (record.typeNameFormat() == QNdefRecord::ExternalRtd ){
            qDebug() << "Got ExtRtd tag";
        } else if (record.isEmpty()) {
            qDebug() << "got empty record...";
        } else {
            qDebug() << "got unknown ndef message type";
        }
    }
}

void NfcHandler::writeTag()
{
    qDebug() << "should write next tag";
    m_writeNextTag = true;
}

void NfcHandler::ndefMessageWritten()
{
    qDebug() << "written successfully";
    emit tagWritten();
}

void NfcHandler::cancelWriteTag()
{
    m_writeNextTag = false;
}
