#ifndef YOUTUBEPROTOCOLHANDLER_H
#define YOUTUBEPROTOCOLHANDLER_H

#include "protocolhandler.h"

class YoutubeProtocolHandler : public ProtocolHandler
{
    Q_OBJECT
public:
    explicit YoutubeProtocolHandler(QObject *parent = 0);

    QString scheme() const;

public slots:
    void execute(const QUrl &uri, bool queue = false);

};

#endif // YOUTUBEPROTOCOLHANDLER_H
