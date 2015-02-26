#ifndef MPRISCONTROLLER_H
#define MPRISCONTROLLER_H

#include <QObject>
#include <QWindow>

#include "../protocolhandlers/protocolmanager.h"

class MprisController : public QObject
{
    Q_OBJECT
public:
    explicit MprisController(ProtocolManager *protocols, QObject *parent = 0);

};

#endif // MPRISCONTROLLER_H
