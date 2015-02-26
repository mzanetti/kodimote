#ifndef PROTOCOLMANAGER_H
#define PROTOCOLMANAGER_H

#include <QObject>
#include <QHash>

#include "protocolhandler.h"

class ProtocolManager : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolManager(QObject *parent = 0);

    void registerProtocol(ProtocolHandler *handler);

    QList<ProtocolHandler*> list() const;

    ProtocolHandler *get(const QString &scheme) const;

private:
    QHash<QString, ProtocolHandler*> m_handlers;
};

#endif // PROTOCOLMANAGER_H
