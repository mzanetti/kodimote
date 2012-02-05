#include "nfchandler.h"
#include "xbmc/xbmc.h"
#include "xbmc/xbmcconnection.h"
#include "xbmc/xbmchostmodel.h"

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
        QNdefNfcUriRecord record;
        XbmcHost *currentHost = XbmcConnection::connectedHost();
        record.setUri(QUrl("xbmc://" + currentHost->address()
                           + ':' + QString::number(currentHost->port())
                           + '/' + currentHost->hostname()
                           + '/' + currentHost->hwAddr().remove(':')));
        QNdefMessage message(record);
        qDebug() << "writing record:" << record.uri();
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
        if (record.isRecordType<QNdefNfcUriRecord>()) {
            QNdefNfcUriRecord uriRecord(record);
            qDebug() << "********** Got URI record:" << uriRecord.uri();
            QUrl uri = uriRecord.uri();
            if(uri.scheme() == "xbmc") {
                qDebug() << "Got and xmbc:// uri" << uri.host() << uri.port() << uri.path();

                XbmcHost host;
                host.setAddress(uri.host());
                host.setPort(uri.port());
                QString path = uri.path().right(uri.path().length() - 1);
                if(path.split('/').count() >= 1) {
                    host.setHostname(path.split('/').first());
                }
                if(path.split('/').count() >= 2) {
                    host.setHwAddr(path.split('/').at(1));
                }
                qDebug() << "Should connect to" << host.address() << ':' << host.port() << host.hostname() << host.hwAddr();
                int index = Xbmc::instance()->hostModel()->insertOrUpdateHost(host);
                Xbmc::instance()->hostModel()->connectToHost(index);
            } else {
                qDebug() << "NDEF uri record not compatible with xbmcremote:" << uriRecord.uri();
                emit tagError(tr("NFC tag is not compatible with Xbmcremote. In order to use it with Xbmcremote you need to write connection information to it."));
            }
        } else if (record.isRecordType<QNdefNfcTextRecord>()) {
            QNdefNfcTextRecord textRecord(record);
            qDebug() << "********** Got Text record:" << textRecord.text();
            if(textRecord.text().startsWith("xbmc:")) {
                qDebug() << "outdated tag detected";
                emit tagError(tr("NFC tag is outdated. In order to use it with Xbmcremote you need to update it by rewriting connection information to it."));
            }

        }else {
            if (record.typeNameFormat() == QNdefRecord::Mime &&
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
            emit tagError(tr("NFC tag is not compatible with Xbmcremote. In order to use it with Xbmcremote you need to write connection information to it."));
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
