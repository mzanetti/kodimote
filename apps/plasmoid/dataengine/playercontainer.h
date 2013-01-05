#ifndef PLAYERCONTAINER_H
#define PLAYERCONTAINER_H

#include <Plasma/DataContainer>

class Player;

class PlayerContainer: public Plasma::DataContainer
{
  Q_OBJECT

public:
  PlayerContainer(QObject *parent = 0);
  
  
public slots:
  void update();
  
private slots:
  void activePlayerChanged();
  void currentItemChanged();
  
private:
  
  Player *m_player;
};

#endif