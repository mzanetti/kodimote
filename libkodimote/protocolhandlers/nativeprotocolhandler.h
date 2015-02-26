#ifndef NATIVEPROTOCOLHANDLER_H
#define NATIVEPROTOCOLHANDLER_H

#include "protocolhandler.h"
#include "protocolmanager.h"

class NativeProtocolHandler : public ProtocolHandler
{
    Q_OBJECT
public:
    explicit NativeProtocolHandler(const QString &scheme, QObject *parent = 0);

    QString scheme() const;

    static void registerAll(ProtocolManager *manager);

public slots:
    void execute(const QUrl &uri, bool queue = false);

private:
    QString m_scheme;
};

#endif // NATIVEPROTOCOLHANDLER_H
