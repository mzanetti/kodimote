#ifndef KEYS_H
#define KEYS_H

#include <QObject>
#include <QVariantMap>

class Keys : public QObject
{
    Q_OBJECT
public:
    explicit Keys(QObject *parent = 0);

public slots:
    void left();
    void right();
    void up();
    void down();
    void home();
    void select();
    void back();

private slots:
    void responseReceived(int id, const QVariantMap &map);

private:
    int m_requestId;
};

#endif // KEYS_H
