#include "protocolmanager.h"

#include "youtubeprotocolhandler.h"

ProtocolManager::ProtocolManager(QObject *parent) :
    QObject(parent)
{
    registerProtocol(new YoutubeProtocolHandler(this));
}

void ProtocolManager::registerProtocol(ProtocolHandler *handler)
{
    m_handlers.insert(handler->scheme(), handler);
}

QList<ProtocolHandler*> ProtocolManager::list() const
{
    return m_handlers.values();
}

ProtocolHandler *ProtocolManager::get(const QString &scheme) const
{
    if (!m_handlers.contains(scheme)) {
        return NULL;
    }

    return m_handlers[scheme];
}
