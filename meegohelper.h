#ifndef MEEGOHELPER_H
#define MEEGOHELPER_H

#include <QObject>
#include <qmsystem2/qmkeys.h>
#include <resource/qt4/policy/resource-set.h>

class MeeGoHelper : public QObject
{
    Q_OBJECT
public:
    explicit MeeGoHelper(QObject *parent = 0);

signals:

private slots:
    void keyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    MeeGo::QmKeys m_keys;
    ResourcePolicy::ResourceSet *m_resouceSet;
    bool m_buttonsAcquired;
};

#endif // MEEGOHELPER_H
