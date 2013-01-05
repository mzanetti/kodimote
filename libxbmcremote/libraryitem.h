#ifndef LIBRARYITEM_H
#define LIBRARYITEM_H

#include "xbmcmodel.h"

#include <QTime>
#include <QObject>
#include <QStandardItem>

class LibraryItem : public XbmcModelItem
{
    Q_OBJECT

    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString filename READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QString fanart READ fanart WRITE setFanart NOTIFY fanartChanged)
    Q_PROPERTY(QString filetype READ fileType WRITE setFileType NOTIFY fileTypeChanged)
    Q_PROPERTY(bool playable READ playable WRITE setPlayable NOTIFY playableChanged)
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(int albumId READ albumId WRITE setAlbumId NOTIFY albumIdChanged)
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(int songId READ songId WRITE setSongId NOTIFY songIdChanged)
    Q_PROPERTY(int musicvideoId READ musicvideoId WRITE setMusicvideoId NOTIFY musicvideoIdChanged)
    Q_PROPERTY(int tvshowId READ tvshowId WRITE setTvshowId NOTIFY tvshowIdChanged)
    Q_PROPERTY(QString tvShow READ tvShow WRITE setTvShow NOTIFY tvShowChanged)
    Q_PROPERTY(int seasonId READ seasonId WRITE setSeasonId NOTIFY seasonIdChanged)
    Q_PROPERTY(int episodeId READ episodeId WRITE setEpisodeId NOTIFY episodeIdChanged)
    Q_PROPERTY(int movieId READ movieId WRITE setMovieId NOTIFY movieIdChanged)
    Q_PROPERTY(QString plot READ plot WRITE setPlot NOTIFY plotChanged)
    Q_PROPERTY(int rating READ rating WRITE setRating NOTIFY ratingChanged)
    Q_PROPERTY(int season READ season WRITE setSeason NOTIFY seasonChanged)
    Q_PROPERTY(int episode READ episode WRITE setEpisode NOTIFY episodeChanged)
    Q_PROPERTY(QString firstAired READ firstAired WRITE setFirstAired NOTIFY firstAiredChanged)
    Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY genreChanged)
    Q_PROPERTY(QString year READ year WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(QString director READ director WRITE setDirector NOTIFY directorChanged)
    Q_PROPERTY(QString tagline READ tagline WRITE setTagline NOTIFY taglineChanged)
    Q_PROPERTY(QString mpaa READ mpaa WRITE setMpaa NOTIFY mpaaChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString instrument READ instrument WRITE setInstrument NOTIFY instrumentChanged)
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(QString mood READ mood WRITE setMood NOTIFY moodChanged)
    Q_PROPERTY(QString born READ born WRITE setBorn NOTIFY bornChanged)
    Q_PROPERTY(QString formed READ formed WRITE setFormed NOTIFY formedChanged)
    Q_PROPERTY(QString died READ died WRITE setDied NOTIFY diedChanged)
    Q_PROPERTY(QString disbanded READ disbanded WRITE setDisbanded NOTIFY disbandedChanged)
    Q_PROPERTY(QTime duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QString durationString READ durationString NOTIFY durationChanged)
    Q_PROPERTY(int durationInSecs READ durationInSecs NOTIFY durationChanged)
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
    Q_PROPERTY(int playcount READ playcount WRITE setPlaycount NOTIFY playcountChanged)
    Q_PROPERTY(QString cast READ cast WRITE setCast NOTIFY castChanged)

public:
    explicit LibraryItem(const QString &title = QString(), const QString &subTitle = QString());

    QString type() const;
    void setType(const QString &type);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QString thumbnail() const;
    void setThumbnail(const QString &thumbnail);

    QString fanart() const;
    void setFanart(const QString &fanart);

    QString fileType() const;
    void setFileType(const QString &fileType);

    bool playable() const;
    void setPlayable(bool playable);

    int artistId() const;
    void setArtistId(int artistId);

    QString artist() const;
    void setArtist(const QString &artist);

    int albumId() const;
    void setAlbumId(int albumId);

    QString album() const;
    void setAlbum(const QString &album);

    int songId() const;
    void setSongId(int songId);

    int musicvideoId() const;
    void setMusicvideoId(int musicvideoId);

    int tvshowId() const;
    void setTvshowId(int tvshowId);

    QString tvShow() const;
    void setTvShow(QString tvShow);

    int seasonId() const;
    void setSeasonId(int seasonId);

    int episodeId() const;
    void setEpisodeId(int episodeId);

    int movieId() const;
    void setMovieId(int movieId);

    QString plot() const;
    void setPlot(const QString &plot);

    int rating() const;
    void setRating(int rating);

    int season() const;
    void setSeason(int season);

    int episode() const;
    void setEpisode(int episode);

    QString firstAired() const;
    void setFirstAired(const QString &firstAired);

    QString genre() const;
    void setGenre(const QString &genre);

    QString year() const;
    void setYear(const QString &year);

    QString director() const;
    void setDirector(const QString &director);

    QString tagline() const;
    void setTagline(const QString &tagline);

    QString mpaa() const;
    void setMpaa(const QString &mpaa);

    QString description() const;
    void setDescription(const QString &description);

    QString instrument() const;
    void setInstrument(const QString &instrument);

    QString style() const;
    void setStyle(const QString &style);

    QString mood() const;
    void setMood(const QString &mood);

    QString born() const;
    void setBorn(const QString &born);

    QString formed() const;
    void setFormed(const QString &formed);

    QString died() const;
    void setDied(const QString &died);

    QString disbanded() const;
    void setDisbanded(const QString &disbanded);

    QTime duration() const;
    QString durationString() const;
    int durationInSecs() const;
    void setDuration(const QTime &duration);

    QString comment() const;
    void setComment(const QString &comment);

    int playcount() const;
    void setPlaycount(int playcount);

    QString cast() const;
    void setCast(const QString &cast);

    virtual QVariant data(int role) const;

signals:
    void typeChanged();
    void fileNameChanged();
    void thumbnailChanged();
    void fanartChanged();
    void fileTypeChanged();
    void playableChanged();
    void artistIdChanged();
    void artistChanged();
    void albumIdChanged();
    void albumChanged();
    void songIdChanged();
    void musicvideoIdChanged();
    void tvshowIdChanged();
    void tvShowChanged();
    void seasonIdChanged();
    void episodeIdChanged();
    void movieIdChanged();
    void plotChanged();
    void ratingChanged();
    void seasonChanged();
    void episodeChanged();
    void firstAiredChanged();
    void genreChanged();
    void yearChanged();
    void directorChanged();
    void taglineChanged();
    void mpaaChanged();
    void descriptionChanged();
    void instrumentChanged();
    void styleChanged();
    void moodChanged();
    void bornChanged();
    void formedChanged();
    void diedChanged();
    void disbandedChanged();
    void durationChanged();
    void commentChanged();
    void playcountChanged();
    void castChanged();

private:
    QString m_type;
    QString m_fileName;
    QString m_thumbnail;
    QString m_fanart;
    QString m_fileType;
    bool m_playable;
    int m_artistId;
    QString m_artist;
    int m_albumId;
    QString m_album;
    int m_songId;
    int m_musicvideoId;
    int m_tvshowId;
    QString m_tvShow;
    int m_seasonId;
    int m_episodeId;
    int m_movieId;
    QString m_plot;
    int m_rating;
    int m_season;
    int m_episode;
    QString m_firstAired;
    QString m_genre;
    QString m_year;
    QString m_director;
    QString m_tagline;
    QString m_mpaa;
    QString m_description;
    QString m_instrument;
    QString m_style;
    QString m_mood;
    QString m_born;
    QString m_formed;
    QString m_died;
    QString m_disbanded;
    QTime m_duration;
    QString m_comment;
    int m_playcount;
    QString m_cast;
};

#endif // LIBRARYITEM_H
