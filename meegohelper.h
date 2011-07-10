#ifndef MEEGOHELPER_H
#define MEEGOHELPER_H

#include <QObject>
#include <qmsystem2/qmkeys.h>

class MeeGoHelper : public QObject
{
    Q_OBJECT
public:
    explicit MeeGoHelper(QObject *parent = 0);

signals:

private slots:
    void keyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State);

private:
    MeeGo::QmKeys m_keys;
};

#endif // MEEGOHELPER_H
