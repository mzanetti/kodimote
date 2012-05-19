#ifndef GESTUREHELPER_H
#define GESTUREHELPER_H

#include <QObject>
#include <QStateMachine>

class GestureHelper : public QObject
{
    Q_OBJECT
    Q_ENUMS(AreaState)

    Q_PROPERTY(AreaState currentState READ currentState NOTIFY currentStateChanged)

    Q_PROPERTY(int startX READ startX WRITE setStartX NOTIFY startXChanged)
    Q_PROPERTY(int startY READ startY WRITE setStartY NOTIFY startYChanged)

    Q_PROPERTY(int realX READ realX WRITE setRealX NOTIFY realXChanged)
    Q_PROPERTY(int realY READ realY WRITE setRealY NOTIFY realYChanged)

    Q_PROPERTY(int restrictedX READ restrictedX NOTIFY realXChanged)
    Q_PROPERTY(int restrictedY READ restrictedY NOTIFY realYChanged)

public:
    enum AreaState {
        AreaCenter,
        AreaTop,
        AreaLeft,
        AreaRight,
        AreaBottom
    };
    explicit GestureHelper(QObject *parent = 0);

    AreaState currentState();
    
    int startX();
    int startY();
    void setStartX(int x);
    void setStartY(int y);

    int realX();
    int realY();
    void setRealX(int x);
    void setRealY(int y);

    int restrictedX();
    int restrictedY();

public slots:
    void reset();

signals:
    void currentStateChanged();
    void startXChanged();
    void startYChanged();
    void realXChanged();
    void realYChanged();
    
private:

    int m_startX;
    int m_startY;
    int m_realX;
    int m_realY;
    int m_limit;
    int m_innerLimit;

    AreaState m_areaState;

};

#endif // GESTUREHELPER_H
