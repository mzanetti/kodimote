#include "xdebug.h"

FakeDevice XDebug::s_fakeDevice;
QList<int> XDebug::s_allowedAreas;

XDebug::XDebug()
{
}

void XDebug::addAllowedArea(int area)
{
    if(!s_allowedAreas.contains(area)) {
        s_allowedAreas.append(area);
    }
}

