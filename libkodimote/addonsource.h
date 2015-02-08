#ifndef ADDONSOURCE_H
#define ADDONSOURCE_H

#include "files.h"

class AddonSource : public Files
{
    Q_OBJECT
public:
    explicit AddonSource(const QString &title, const QString &mediaType, const QString &dir, KodiModel *parent = 0);

    QString title() const;
    KodiModel *enterItem(int index);

    MediaFormat mediaFormat() const {
        if (m_mediaType == "music") {
            return MediaFormatAudio;
        } else if (m_mediaType == "video") {
            return MediaFormatVideo;
        } else if (m_mediaType == "pictures") {
            return MediaFormatPictures;
        }
        return MediaFormatUnknown;
    }

protected:
    QString parseTitle(const QString &title) const;
    bool filterFile(const QVariantMap &item) const;

private:
    QString m_title;
};

#endif // ADDONSOURCE_H
