#include "settings.h"

#include <QSettings>

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

bool Settings::themeInverted() const
{
    QSettings settings("xbmcremote", "xbmcremote");
    return settings.value("InvertTheme", false).toBool();
}

void Settings::setThemeInverted(bool inverted)
{
    QSettings settings("xbmcremote", "xbmcremote");
    settings.setValue("InvertTheme", inverted);
}
