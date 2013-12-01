/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "nfchandler.h"
#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xbmcconnection.h"
#include "libxbmcremote/xbmchostmodel.h"

#include <QDebug>
#include <QUrl>
#include <QVariant>
#include <QStringList>

#include <QNdefNfcTextRecord>
#include <QNdefNfcUriRecord>
QTM_USE_NAMESPACE

NfcHandler::NfcHandler(QObject *parent) :
    QObject(parent),
    m_writeNextTag(false),
    m_writeError(false)
{
    m_manager.setTargetAccessModes(QNearFieldManager::NdefReadTargetAccess | QNearFieldManager::NdefWriteTargetAccess);
    qDebug() << "Started NFC tag detection with status:" << m_manager.startTargetDetection();

    connect(&m_manager, SIGNAL(targetDetected(QNearFieldTarget*)), SLOT(tagDetected(QNearFieldTarget*)));
}

bool NfcHandler::isAvailable()
{
    return m_manager.isAvailable();
}

void NfcHandler::tagDetected(QNearFieldTarget *tag)
{
    qDebug() << "NFC tag detected:" << tag->type();
    if(!m_tagList.contains(tag)) {
        connect(tag, SIGNAL(requestCompleted(QNearFieldTarget::RequestId)), this, SLOT(requestCompleted(QNearFieldTarget::RequestId)));
        connect(tag, SIGNAL(ndefMessageRead(QNdefMessage)), this, SLOT(ndefMessageRead(QNdefMessage)));
        connect(tag, SIGNAL(ndefMessagesWritten()), SLOT(ndefMessageWritten()));
        connect(tag, SIGNAL(error(QNearFieldTarget::Error,QNearFieldTarget::RequestId)), SLOT(error(QNearFieldTarget::Error,QNearFieldTarget::RequestId)));
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
            } else {
                emit tagError(tr("NFC tag is not compatible with Xbmcremote. In order to use it with Xbmcremote you need to write connection information to it."));
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
    m_writeError = false;
}

void NfcHandler::ndefMessageWritten()
{
    qDebug() << "written successfully";
    if(m_writeError) {
        emit tagWritten(tr("Tag written successfully. NOTE: This tag does not offer enough space to write all informations. Wake on LAN will not work with this tag."));
    } else {
        emit tagWritten(tr("Tag written successfully."));
    }
}

void NfcHandler::error(QNearFieldTarget::Error error, const QNearFieldTarget::RequestId &id)
{
    Q_UNUSED(id);

    qDebug() << "tag write error:" << error;
    if(m_writeError) {
        emit tagWritten(tr("Error writing NFC tag."));
        return;
    }

    qDebug() << "Failed to write NFC tag for the first time. It might be a 48bytes chip and our URI is too long for it. Retrying without MAC address. WakeonLan will not work with this Tag.";
    m_writeError = true;

    QNearFieldTarget *tag = qobject_cast<QNearFieldTarget*>(sender());

    QNdefNfcUriRecord record;
    XbmcHost *currentHost = XbmcConnection::connectedHost();
    record.setUri(QUrl("xbmc://" + currentHost->address()
                       + ':' + QString::number(currentHost->port())
                       + '/' + currentHost->hostname()
                       + '/'));
    QNdefMessage message(record);
    qDebug() << "writing record:" << record.uri();

    tag->writeNdefMessages(QList<QNdefMessage>() << message);

}

void NfcHandler::cancelWriteTag()
{
    m_writeNextTag = false;
}
