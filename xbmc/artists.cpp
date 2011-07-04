#include "artists.h"
#include "xbmcconnection.h"

#include "albums.h"

Artists::Artists(XbmcModel *parent) :
    XbmcModel(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("AudioLibrary.GetArtists");
}

Artists::~Artists()
{
}

void Artists::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got artists:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("artists").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData("directory", Qt::UserRole);
        item->setData(itemMap.value("artistid").toInt(), Qt::UserRole + 100);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int Artists::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant Artists::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_list.at(index.row())->text();
    case Qt::UserRole+1:
        return "directory";
    case Qt::UserRole+2:
        return "";
    }
    return QVariant();
}

XbmcModel *Artists::enterItem(int index)
{
    return new Albums(m_list.at(index)->data(Qt::UserRole + 100).toInt(), this);
}

void Artists::playItem(int index)
{
    qDebug() << "Artists::play() not implemented yet";
}

QString Artists::title() const
{
    return "Artists";
}
