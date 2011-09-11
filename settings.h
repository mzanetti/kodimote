#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool themeInverted READ themeInverted WRITE setThemeInverted)
public:
    explicit Settings(QObject *parent = 0);

    void setThemeInverted(bool inverted);
    bool themeInverted() const;

};

#endif // SETTINGS_H
