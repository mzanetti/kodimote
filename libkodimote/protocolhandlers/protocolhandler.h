#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QUrl>

#include "../playlistitem.h"

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(QObject *parent = 0);

    virtual QString scheme() const = 0;

public slots:
    virtual void execute(const QUrl &uri, bool queue = false) = 0;

};

#endif // PROTOCOLHANDLER_H
