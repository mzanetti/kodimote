/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include <QObject>
#include <QHash>
#include <QPointer>
#include <QBuffer>
#include <QThread>
#include <QMutex>
#include <QVariantMap>
#include <QSize>
#include <QTimer>

class QNetworkReply;

class ImageFetchJob;
class QFile;

// Can't be ImageCache because invoker on Harmattan causes a name-clash and breaks method-invokation
class KodiImageCache : public QObject
{
    Q_OBJECT
public:
    explicit KodiImageCache(QObject *parent = 0);
    
    bool contains(const QString &image, int cacheId, QString &cachedFile);

public slots:
    /**
      * Fetch "image" and report back to "callbackObject" by invoking "callbackFunction"
      * Returns a unique job id
      * "callbackFunction" must be invokable and receive a int as parameter which will contain the returned job id
      * If "scaleTo" is given the image will be scaled to given size before storing to the cache
      * Optionally there can be used multiple caches to save the same image in different sizes
      * This can be useful to cache small versions in lists that need to be fast and contain lots of images,
      * but large versions for places where fullscreen images are needed)
      */
    int fetch(const QString &image, QObject *callbackObject, const QString &callbackFunction, const QSize &scaleTo = QSize(0, 0), int cacheId = 0);

private slots:
    void imageFetched();

    void fetchNext(ImageFetchJob *job);
    void downloadPrepared(const QVariantMap &map);
    void imageScaled();
private:
    static QString cacheKey(const QString &image, int cacheId);
    static QString cachedFile(const QString &path, const QString &image);
    static QString cachePath(int cacheId);

    int m_jobId;

    QHash<QString, ImageFetchJob*> m_jobs;

    QHash<QString, QPair<bool, QString> > m_cacheFiles;
    QHash<int, QString> m_fetchQueue;

    bool m_doubleDecode;
};

class ImageFetchJob : public QObject
{
public:
    ImageFetchJob(int id, int cacheId, const QString &imageName, const QString &cachedFile, const QSize &scaleTo = QSize(0, 0)) :
        m_id(id),
        m_cacheId(cacheId),
        m_imageName(imageName),
        m_cachedFile(cachedFile),
        m_scalingSize(scaleTo)
    {
    }
    ~ImageFetchJob()
    {
    }

    class Callback
    {
    public:
        Callback(QPointer<QObject> object, const QString &method):
            m_object(object), m_method(method) {}

        QPointer<QObject> object() const { return m_object; }
        QString method() const { return m_method; }

    private:
        QPointer<QObject> m_object;
        QString m_method;
    };

    int id() { return m_id; }
    int cacheId() { return m_cacheId; }
    QString imageName() const { return m_imageName; }
    QString cachedFile() const { return m_cachedFile; }
    QSize scaleTo() const { return m_scalingSize; }
    QList<Callback> callbacks() const { return m_callbacks; }

    void appendCallback(QPointer<QObject> object, const QString &method) { m_callbacks.append(Callback(object, method)); }

private:
    int m_id;
    int m_cacheId;
    QString m_imageName;
    QString m_cachedFile;
    QSize m_scalingSize;
    QList<Callback> m_callbacks;
};

#endif // IMAGECACHE_H
