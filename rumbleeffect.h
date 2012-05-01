#ifndef RUMBLEEFFECT_H
#define RUMBLEEFFECT_H

#include <QObject>
#include <QFeedbackHapticsEffect>
QTM_USE_NAMESPACE

class RumbleEffect : public QObject
{
    Q_OBJECT
public:
    explicit RumbleEffect(QObject *parent = 0);
    
public slots:
    void start();

private:
    QFeedbackHapticsEffect *m_effect;
};

#endif // RUMBLEEFFECT_H
