#include "shares.h"
#include "files.h"
#include "xbmcconnection.h"

Shares::Shares(const QString &mediatype):
    XbmcModel(0),
    m_mediaType(mediatype)
{
    QVariantMap params;

//    QVariant media(mediaString());
    params.insert("media", mediatype);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_requestId = XbmcConnection::sendCommand("Files.GetSources", params);
}

void Shares::responseReceived(int id, const QVariantMap &rsp)
{

//    qDebug() << "Files reponse:" << response;
    if(id != m_requestId) {
        return;
    }
    QList<QStandardItem*> list;
    qDebug() << "got shares:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("shares").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("file").toString(), Qt::UserRole + 100);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(QStandardItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();
}

int Shares::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Shares::data(const QModelIndex &index, int role) const
{
    if(role == Qt::UserRole+1) {
        return "directory";
    }
    return m_list.at(index.row())->data(role);
}

XbmcModel *Shares::enterItem(int index)
{
    return new Files(m_mediaType, m_list.at(index)->data(Qt::UserRole+100).toString(), this);
}

void Shares::playItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Playing whole shares is not supported";
}

QString Shares::title() const
{
    if(m_mediaType == "music") {
        return "Music Files";
    } else {
        return "Video Files";
    }
}
