/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef FILES_H
#define FILES_H

#include <QAbstractItemModel>
#include <QStringList>

class Player;

class Files : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentDir READ currentDir NOTIFY dirChanged)
    Q_PROPERTY(MediaType mediaType READ mediaType WRITE setMediaType NOTIFY mediaTypeChanged)

public:
    enum MediaType {
        MediaTypeAudio,
        MediaTypeVideo
    };

    enum PlayBehavior {
        PlayBehaviorSingle,
        PlayBehaviorAll
    };

    explicit Files(MediaType mediaType, Player *player, QObject *parent = 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QString currentDir() const;
    MediaType mediaType() const;

signals:
    void dirChanged();
    void mediaTypeChanged();

public slots:
    void responseReceived(int, const QVariantMap &rsp);

    void enterDir(const QString &dir);

    void goUp(int levels);

    void listShares();

    void setMediaType(MediaType &mediaType);

private:
    enum Request {
        RequestSources,
        RequestDirectory
    };
    QMap<int, Request> m_requestMap;

    MediaType m_mediaType;
    Player *m_player;
    QList<QPair<QString, QString> > m_filesList;
    QMap<QString, QString> m_sharesMap;
    QString m_currentDir;
    QString m_currentDirLabel;
    PlayBehavior m_playBehavior;

    QString mediaString();
};

#endif // FILES_H
