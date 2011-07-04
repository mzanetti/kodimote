#include "keys.h"
#include "xbmcconnection.h"

Keys::Keys(QObject *parent) :
    QObject(parent)
{
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

void Keys::home()
{
    XbmcConnection::sendCommand("Input.Home");
}

void Keys::select()
{
    XbmcConnection::sendCommand("Input.Select");
}

void Keys::responseReceived(int id, const QVariantMap &map)
{
    // TODO...
}
