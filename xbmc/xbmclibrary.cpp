#include "xbmclibrary.h"
#include "libraryitem.h"
#include "xbmchostmodel.h"
#include "xbmcconnection.h"
#include "xbmcdownload.h"

#include <QTimer>

XbmcLibrary::XbmcLibrary(XbmcModel *parent) :XbmcModel(parent), m_deleteAfterDownload(false)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(slotResponseReceived(int, QVariantMap)));

    // Refresh the model automatically on the next event loop run.
    // This is to give QML time to create the object and set properties before the refresh
    QTimer::singleShot(0, this, SLOT(refresh()));
}

XbmcLibrary::~XbmcLibrary()
{
}

QVariant XbmcLibrary::data(const QModelIndex &index, int role) const
{
    return m_list.at(index.row())->data(role);
}

XbmcModel* XbmcLibrary::exit()
{
    deleteLater();
    return m_parentModel;
}

QVariant XbmcLibrary::get(int row, const QString &roleName)
{
    return data(index(row, 0, QModelIndex()), roleNames().key(roleName.toAscii()));
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
    QVariantMap params;
    params.insert("path", item->fileName());
    int id = XbmcConnection::sendCommand("Files.PrepareDownload", params);

    m_requestMap.insert(id, RequestDownload);
    m_downloadMap.insert(id, download);
}

void XbmcLibrary::slotResponseReceived(int id, const QVariantMap &rsp)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    switch(m_requestMap.value(id)) {
    case RequestDownload: {
        XbmcDownload *download = m_downloadMap.take(id);
        if(rsp.contains("error")) {
            delete download;
            break;
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
    }

    m_requestMap.remove(id);
}
