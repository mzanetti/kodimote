#include "albums.h"
#include "xbmcconnection.h"

#include "songs.h"

Albums::Albums(int artistId, XbmcModel *parent) :
    XbmcModel(parent),
    m_artistId(artistId)
{
    QVariantMap params;
    if(artistId != -1) {
      params.insert("artistid", artistId);
    }
    QVariantList fields;
    fields.append("artist");
    params.insert("fields", fields);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    params.insert("sort", sort);
    m_request = XbmcConnection::sendCommand("AudioLibrary.GetAlbums", params);
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
}

Albums::~Albums()
{
}

void Albums::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got albums:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("albums").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("artist").toString(), Qt::UserRole + 2);
        item->setData(itemMap.value("albumid").toInt(), Qt::UserRole + 100);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int Albums::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant Albums::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::UserRole+1:
        return "directory";
    }
    return m_list.at(index.row())->data(role);
}

XbmcModel* Albums::enterItem(int index)
{
    return new Songs(m_artistId, m_list.at(index)->data(Qt::UserRole + 100).toInt(), this);
}

void Albums::playItem(int index)
{
    qDebug() << "Albums::play() not implemented yet";
}

QString Albums::title() const
{
    return "Albums";
}
