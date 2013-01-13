#include "xbmclibrary.h"
#include "libraryitem.h"
#include "xbmchostmodel.h"
#include "xbmcconnection.h"
#include "xbmcdownload.h"
#include "xbmc.h"
#include "player.h"
#include "audioplayer.h"
#include "videoplayer.h"

#include "libraryitem.h"

#include <QTimer>
#include <QFileInfo>

XbmcLibrary::XbmcLibrary(XbmcModel *parent) :XbmcModel(parent), m_deleteAfterDownload(false)
{
    // Refresh the model automatically on the next event loop run.
    // This is to give QML time to create the object and set properties before the refresh
    QTimer::singleShot(0, this, SLOT(refresh()));

    connect(Xbmc::instance()->audioPlayer(), SIGNAL(currentItemChanged()), SLOT(currentItemChanged()));
    connect(Xbmc::instance()->videoPlayer(), SIGNAL(currentItemChanged()), SLOT(currentItemChanged()));
    connect(Xbmc::instance()->audioPlayer(), SIGNAL(stateChanged()), SLOT(currentItemChanged()));
    connect(Xbmc::instance()->videoPlayer(), SIGNAL(stateChanged()), SLOT(currentItemChanged()));
}

XbmcLibrary::~XbmcLibrary()
{
}

QVariant XbmcLibrary::data(const QModelIndex &index, int role) const
{
    if(role == RolePlayingState) {
        if(!Xbmc::instance()->activePlayer() || !Xbmc::instance()->activePlayer()->currentItem()) {
            return "";
        }
        LibraryItem *thisItem = qobject_cast<LibraryItem*>(m_list.at(index.row()));
        LibraryItem *currentItem = Xbmc::instance()->activePlayer()->currentItem();
//        qDebug() << "$$$$$$1" << thisItem->artistId() << thisItem->albumId() << thisItem->songId() << thisItem->movieId() << thisItem->episodeId() << thisItem->fileName() << thisItem->channelId();
//        qDebug() << "$$$$$$2" << currentItem->artistId() << currentItem->albumId() << currentItem->songId() << currentItem->movieId() << currentItem->episodeId() << currentItem->fileName() << currentItem->channelId();
        if(currentItem->artistId() == -1 &&
                    currentItem->songId() == -1 &&
                    currentItem->albumId() == -1 &&
                    currentItem->movieId() == -1 &&
                    currentItem->episodeId() == -1 &&
                    currentItem->channelId() == -1) {

            if(thisItem->fileName() == currentItem->fileName()) {
                return Xbmc::instance()->activePlayer()->state();
            }
            return "";
        }
        if(thisItem->artistId() == currentItem->artistId() &&
               thisItem->albumId() == currentItem->albumId() &&
               thisItem->songId() == currentItem->songId() &&
               thisItem->movieId() == currentItem->movieId() &&
               thisItem->episodeId() == currentItem->episodeId() &&
               thisItem->channelId() == currentItem->channelId()) {
            return Xbmc::instance()->activePlayer()->state();
        }
        return "";
    }


    return XbmcModel::data(index, role);
}

XbmcModel* XbmcLibrary::exit()
{
    deleteLater();
    return m_parentModel;
}

QVariant XbmcLibrary::get(int row, const QString &roleName)
{
    return data(index(row, 0, QModelIndex()), roleNames().key(roleName.toLatin1()));
}

XbmcModelItem *XbmcLibrary::getItem(int index)
{
    qDebug() << "returning: " << m_list.at(index);
    return m_list.at(index);
}

void XbmcLibrary::download(int index, const QString &path)
{
    qDebug() << "This model does not support downloading";
}

void XbmcLibrary::setDeleteAfterDownload(bool deleteAfterDownload)
{
    m_deleteAfterDownload = deleteAfterDownload;
}

bool XbmcLibrary::deleteAfterDownload() const
{
    return m_deleteAfterDownload;
}

void XbmcLibrary::startDownload(int index, XbmcDownload *download)
{
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(index));

    QFileInfo fileInfo(item->fileName().replace('\\', '/')); // Make sure it works on Windoze too
    download->setDestination(download->destination() + fileInfo.fileName());

    QVariantMap params;
    params.insert("path", item->fileName());
    int id = XbmcConnection::sendCommand("Files.PrepareDownload", params, this, "downloadReceived");
    m_downloadMap.insert(id, download);
}

void XbmcLibrary::downloadReceived(const QVariantMap &rsp)
{
    int id = rsp.value("id").toInt();
    XbmcDownload *download = m_downloadMap.take(id);
    if(rsp.contains("error")) {
        delete download;
        return;
    }
    QString path = rsp.value("result").toMap().value("details").toMap().value("path").toString();
    qDebug() << "Downloading" << path;

    download->setSource(path);

    XbmcConnection::download(download);

    if(m_deleteAfterDownload && m_downloadMap.isEmpty()) {
        qDebug() << "Deleting download model";
        deleteLater();
    }
}

void XbmcLibrary::currentItemChanged()
{
    emit dataChanged(index(0, 0, QModelIndex()), index(m_list.count()-1, 0, QModelIndex()));
}
