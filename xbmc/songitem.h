#ifndef SONGITEM_H
#define SONGITEM_H

#include <QString>
#include <QTime>

namespace Xbmc
{


class SongItem
{
public:
    SongItem(int songid = -1, const QString &label = QString());

    void setLabel(const QString &label);
    void setSongId(int songId);
    void setFile(const QString &file);
    void setTitle(const QString &title);
    void setArtist(const QString &artist);
    void setAlbum(const QString &album);
    void setThumbnail(const QString &thumbnail);
    void setFanart(const QString &fanart);
    void setDuration(const QTime &duration);


    int songId() const;
    QString file() const;
    QString label() const;
    QString title() const;
    QString artist() const;
    QString album() const;
    QString thumbnail() const;
    QString fanart() const;
    QTime duration() const;

private:
    int m_songId;
    QString m_file;
    QString m_label;
    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_thumbnail;
    QString m_fanart;
    QTime m_duration;
};

}
#endif // SONGITEM_H
