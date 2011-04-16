#ifndef FILES_H
#define FILES_H

#include <QAbstractItemModel>
#include <QStringList>

namespace Xbmc
{

class Player;

class Files : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentDir READ currentDir NOTIFY dirChanged)
public:
    explicit Files(Player *player, QObject *parent = 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QString currentDir() const;

signals:
    void dirChanged();

public slots:
    void responseReceived(int, const QVariantMap &rsp);

    void enterDir(const QString &dir);

    void goUp(int levels);

    void listShares();

private:
    enum Request {
        RequestSources,
        RequestDirectory
    };
    QMap<int, Request> m_requestMap;

    Player *m_player;
    QList<QPair<QString, QString> > m_filesList;
    QMap<QString, QString> m_sharesMap;
    QString m_currentDir;
    QString m_currentDirLabel;
};

}

#endif // FILES_H
