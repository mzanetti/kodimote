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

#ifndef NFCHANDLER_H
#define NFCHANDLER_H

#include <QObject>

#include <QNearFieldManager>
#include <QNdefMessage>
#include <QNearFieldTarget>
#include <QNdefNfcTextRecord>
#include <QNdefNfcUriRecord>
QTM_USE_NAMESPACE

class NfcHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isAvailable READ isAvailable NOTIFY availableChanged)

public:
    explicit NfcHandler(QObject *parent = 0);

    bool isAvailable();

signals:
    void availableChanged();
    void tagWritten(const QString &statusText);
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
    QNearFieldManager m_manager;
    QList<QNearFieldTarget *> m_tagList;
    bool m_writeNextTag;
    bool m_writeError;
};

#endif // NFCHANDLER_H
