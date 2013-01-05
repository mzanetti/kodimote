#include "playercontainer.h"

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/player.h"
#include "libxbmcremote/audioplayer.h"
#include "libxbmcremote/videoplayer.h"
#include "libxbmcremote/libraryitem.h"

#include <QDebug>


PlayerContainer::PlayerContainer(QObject *parent):
  Plasma::DataContainer(parent),
  m_player(0)
{

    connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(activePlayerChanged()));
    connect(Xbmc::instance(), SIGNAL(activePlayerChanged()), SLOT(activePlayerChanged()));
    
    activePlayerChanged();
}

void PlayerContainer::activePlayerChanged()
{
  
      disconnect(this, SLOT(update()));
      disconnect(this, SLOT(currentItemChanged()));
      

      if(Xbmc::instance()->activePlayer()) {
	connect(Xbmc::instance()->activePlayer(), SIGNAL(stateChanged()), SLOT(update()));
	connect(Xbmc::instance()->activePlayer(), SIGNAL(currentItemChanged()), SLOT(currentItemChanged()));
      }
      
      update();
}

void PlayerContainer::currentItemChanged()
{
    if(!Xbmc::instance()->activePlayer()) {
	setData("title", "");
        checkForUpdate();
	return;
    }
    
    LibraryItem *currentItem = Xbmc::instance()->activePlayer()->currentItem();
    setData("title", currentItem->title());

    checkForUpdate();

}

void PlayerContainer::update()
{
    qDebug() << "updated!!!!";
    m_player = Xbmc::instance()->activePlayer();
    
    if(m_player == 0) {
	setData("state", "none");
	setData("title", "");
    } else {
	setData("state", m_player->state());
	setData("title", m_player->currentItem()->title());
    }
    
    currentItemChanged();
}

//#include "playercontainer.moc"
