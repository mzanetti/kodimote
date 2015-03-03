#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include "files.h"

class Playlists : public Files
{
    Q_OBJECT
public:
    explicit Playlists(const QString &title, const QString &mediaType, const QString &dir, KodiModel *parent = 0);

    QString title() const;
    KodiModel *enterItem(int index);

private:
    QString m_title;
};

#endif // PLAYLISTS_H
