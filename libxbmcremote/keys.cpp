/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "keys.h"
#include "xbmcconnection.h"

Keys::Keys(QObject *parent) :
    QObject(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
}

void Keys::left()
{
    XbmcConnection::sendCommand("Input.Left");
}

void Keys::right()
{
    XbmcConnection::sendCommand("Input.Right");
}

void Keys::up()
{
    XbmcConnection::sendCommand("Input.Up");
}

void Keys::down()
{
    XbmcConnection::sendCommand("Input.Down");
}

void Keys::back()
{
    XbmcConnection::sendCommand("Input.Back");
}

void Keys::osd()
{
    XbmcConnection::sendCommand("Input.ShowOSD");
}

void Keys::contextMenu()
{
    XbmcConnection::sendCommand("Input.ContextMenu");
}

void Keys::info()
{
    XbmcConnection::sendCommand("Input.Info");
}

void Keys::fullscreen()
{
    QVariantMap params;
    params.insert("action", "fullscreen");
    XbmcConnection::sendCommand("Input.ExecuteAction", params);
}

void Keys::keyboardKey(const QString &key)
{
    if(key.isEmpty()) {
        return;
    }
    int keyValue = 0xF100 + key[0].digitValue();
    QString cmd = "SendKey(" + QString::number(keyValue) + ")";
    qDebug() << key[0] << "sending command" << cmd;
    XbmcConnection::sendLegacyCommand(cmd);
}

void Keys::backspace()
{
    QVariantMap params;
    params.insert("action", "backspace");
    XbmcConnection::sendCommand("Input.ExecuteAction", params);
}

void Keys::home()
{
    XbmcConnection::sendCommand("Input.Home");
}

void Keys::select()
{
    XbmcConnection::sendCommand("Input.Select");
}

void Keys::sendText(QString text, bool done)
{
    QVariantMap map;
    map.insert("text", text);
    map.insert("done", done);
    XbmcConnection::sendCommand("Input.SendText", map);
}

void Keys::receivedAnnouncement(const QVariantMap &map)
{
    QString method = map.value("method").toString();
    QVariantMap data = map.value("params").toMap().value("data").toMap();

    if(method == "Input.OnInputRequested") {
        QString title = data.value("title").toString();
        QString type = data.value("type").toString();
        QString value = data.value("value").toString();
        emit inputRequested(title, type, value);
    }
    else if(method == "Input.OnInputFinished") {
        emit inputFinished();
    }
}

QString Keys::formatTime(int hours, int minutes)
{
    return QString("%1:%2").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0'));
}
