#ifndef MPRISCONTROLLER_H
#define MPRISCONTROLLER_H

#include <QObject>
#include <QWindow>

class MprisController : public QObject
{
    Q_OBJECT
public:
    explicit MprisController(QObject *parent = 0);

};

#endif // MPRISCONTROLLER_H
