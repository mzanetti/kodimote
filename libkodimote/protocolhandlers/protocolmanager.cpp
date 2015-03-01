#include "protocolmanager.h"

#include <QUrlQuery>

#include "youtubeprotocolhandler.h"
#include "nativeprotocolhandler.h"

ProtocolManager::ProtocolManager(QObject *parent) :
    QObject(parent)
{
    NativeProtocolHandler::registerAll(this);
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

void ProtocolManager::execute(const QUrl &url)
{
    if (!m_handlers.contains(url.scheme())) {
        return;
    }

    ProtocolHandler *handler = m_handlers[url.scheme()];

    QUrlQuery query(url);
    handler->execute(url, query.hasQueryItem("queue"));
}
