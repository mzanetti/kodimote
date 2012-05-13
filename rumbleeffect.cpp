#include "rumbleeffect.h"
#include <QDebug>
#include <QApplication>

RumbleEffect::RumbleEffect(QObject *parent) :
    QObject(parent),
    m_effect(0),
    m_disabled(false)
{
    if(QApplication::arguments().contains("-r")) {
        m_disabled = true;
    }
}

void RumbleEffect::start()
{
    if(m_disabled) {
        return;
    }
    // Do not load the hapticseffect as long as we don't need it. It'll kill the debug output!
    if(!m_effect) {
        qDebug() << "Creating Rumble effect. Unfortunately it has a bug destroying the debug output. If you want to have logs, don't use the keypad or use the command line argument \"-r\".";
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
