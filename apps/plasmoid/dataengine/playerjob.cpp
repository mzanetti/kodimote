#include "playerjob.h"

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/player.h"

#include <QDebug>

PlayerJob::PlayerJob(const QString& operation,
                    QMap<QString,QVariant>& parameters,
                    QObject* parent):
  Plasma::ServiceJob(operation, operation, parameters, parent)
{


}


void PlayerJob::start()
{
  qDebug() << "should start job:" << operationName();

  if(!Xbmc::instance()->activePlayer()) {
    return;
  }
  
    if(operationName() == "PlayPause") {
      Xbmc::instance()->activePlayer()->playPause();
    } else if(operationName() == "SkipNext") {
      Xbmc::instance()->activePlayer()->skipNext();
    } else if(operationName() == "SkipPrevious") {
      Xbmc::instance()->activePlayer()->skipPrevious();
    } else if(operationName() == "SeekForward") {
      Xbmc::instance()->activePlayer()->seekForward();
    } else if(operationName() == "SeekBackward") {
      Xbmc::instance()->activePlayer()->seekBackward();
    }
}
