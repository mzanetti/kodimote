#include "meegohelper.h"
#include "xbmc/xbmc.h"

#include <QDebug>
#include <QApplication>
#include <policy/audio-resource.h>

MeeGoHelper::MeeGoHelper(QObject *parent) :
    QObject(parent),
    m_resouceSet(new ResourcePolicy::ResourceSet("player"))
{
    connect(&m_keys, SIGNAL(keyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State)), SLOT(keyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State)));

    m_resouceSet->addResource(ResourcePolicy::ScaleButtonType);
    QApplication::instance()->installEventFilter(this);

    m_resouceSet->acquire();
    m_buttonsAcquired = true;
}

bool MeeGoHelper::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate) {
        m_resouceSet->release();
        m_buttonsAcquired = false;
    } else if(event->type() == QEvent::WindowActivate){
        m_resouceSet->acquire();
        m_buttonsAcquired = true;
    }
    return QObject::eventFilter(obj, event);
}

void MeeGoHelper::keyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state)
{
    // only handle presses, no releases for now...
    if(!m_buttonsAcquired || state != MeeGo::QmKeys::KeyDown) {
        return;
    }

    qDebug() << "keyEvent:" << key << state;
    switch(key) {
    case MeeGo::QmKeys::VolumeUp:
        Xbmc::instance()->setVolume(Xbmc::instance()->volume() + 5);
        break;
    case MeeGo::QmKeys::VolumeDown:
        Xbmc::instance()->setVolume(Xbmc::instance()->volume() - 5);
        break;
    }
}
