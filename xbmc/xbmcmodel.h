#ifndef XBMCMODEL_H
#define XBMCMODEL_H

#include <QAbstractItemModel>
#include <QStandardItem>

class XbmcModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
public:
    enum Roles {
        RoleFileType = Qt::UserRole + 1,
        RoleSubtitle = Qt::UserRole + 2,
        RoleDuration = Qt::UserRole + 3,
        RolePlayable = Qt::UserRole + 4,

        RoleSongId = Qt::UserRole + 100,
        RoleAlbumId = Qt::UserRole + 101,
        RoleArtistId = Qt::UserRole + 102,
        RoleFileName = Qt::UserRole + 103,
        RoleMovieId = Qt::UserRole + 104,
        RoleTvShowId = Qt::UserRole + 105,
        RoleSeasonId = Qt::UserRole + 106,
        RoleEpisodeId = Qt::UserRole + 107,
        RoleMusicVideoId = Qt::UserRole + 108
    };

    explicit XbmcModel(XbmcModel *parent = 0);
    virtual ~XbmcModel();
    Q_INVOKABLE XbmcModel *parentModel() const;

    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    Q_INVOKABLE virtual XbmcModel *enterItem(int index) = 0;
    Q_INVOKABLE virtual void playItem(int index) = 0;
    Q_INVOKABLE virtual void addToPlaylist(int index) = 0;
    Q_INVOKABLE XbmcModel *exit();

    Q_INVOKABLE virtual QString title() const = 0;

    Q_INVOKABLE int findItem(const QString &string, bool caseSensitive = false);

signals:
    void titleChanged();

protected:
    XbmcModel *m_parentModel;
    QList<QStandardItem*> m_list;
};

#endif // XBMCMODEL_H
