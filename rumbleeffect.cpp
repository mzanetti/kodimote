#include "rumbleeffect.h"
#include <QDebug>

RumbleEffect::RumbleEffect(QObject *parent) :
    QObject(parent),
    m_effect(0)
{
}

void RumbleEffect::start()
{
    // Do not load the hapticseffect as long as we don't need it. It'll kill the debug output!
    if(!m_effect) {
        qDebug() << "Creating Haptics effect. Unfortunately it has a bug destroying the debug output. If you want to have logs, don't use the keypad.";
        m_effect = new QFeedbackHapticsEffect(this);
        m_effect->setAttackIntensity(0);
        m_effect->setAttackTime(250);
        m_effect->setIntensity(1.0);
        m_effect->setDuration(100);
        m_effect->setFadeTime(250);
        m_effect->setFadeIntensity(0);
    }
    m_effect->start();
}
