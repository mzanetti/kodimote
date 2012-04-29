#ifndef SYMBIANHELPER_H
#define SYMBIANHELPER_H

#include <QObject>

class Settings;

class SymbianHelper : public QObject
{
    Q_OBJECT
public:
    explicit SymbianHelper(Settings *settings, QObject *parent = 0);

signals:

private slots:
    void connectionChanged(bool connected);
    void hostRemoved();

private:
    Settings *m_settings;
};

#endif // SYMBIANHELPER_H
