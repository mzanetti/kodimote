#ifndef FILES_H
#define FILES_H

#include "xbmcmodel.h"

class Files : public XbmcModel
{
    Q_OBJECT
public:
    explicit Files(const QString &mediaType, const QString &dir, XbmcModel *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    XbmcModel *enterItem(int index);
    void playItem(int index);

    QString title() const;

private slots:
    void responseReceived(int id, const QVariantMap &rsp);

private:
    int m_requestId;
    QString m_mediaType;
    QString m_dir;
};

#endif // FILES_H
