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

class QNetworkReply;

class ImageFetchJob;
class QFile;

// Can't be ImageCache because invoker on Harmattan causes a name-clash and breaks method-invokation
class XbmcImageCache : public QThread
{
    Q_OBJECT
public:
    explicit XbmcImageCache(QObject *parent = 0);
    
    bool contains(const QString &image, int cacheId);
    QString cachedFile(const QString &image, int cacheId);

    static QString cachePath(int cacheId);

    void run();

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
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void fetchNext();
    void downloadPrepared(const QVariantMap &map);

    void cleanupAndTriggerNext();
private:
    int m_jobId;

    ImageFetchJob *m_currentJob;
    QList<ImageFetchJob*> m_downloadQueue;
    QList<ImageFetchJob*> m_toBeNotifiedList;
    QMutex m_mutex;

    QList<QHash<QString, bool> > m_cacheFiles;

};

class ImageFetchJob
{
public:
    ImageFetchJob(int id, int cacheId, const QString &imageName, QPointer<QObject> callbackObject, const QString &callbackMethod, const QSize &scaleTo = QSize(0, 0)) :
        m_id(id),
        m_cacheId(cacheId),
        m_imageName(imageName),
        m_callbackObject(callbackObject),
        m_callbackMethod(callbackMethod),
        m_scalingSize(scaleTo)
    {
    }
    ~ImageFetchJob()
    {
    }

    int id() { return m_id; }
    int cacheId() { return m_cacheId; }
    QString imageName() const { return m_imageName; }
    QPointer<QObject> callbackObject() const { return m_callbackObject; }
    QString callbackMethod() const { return m_callbackMethod; }
    QSize scaleTo() const { return m_scalingSize; }

    void appendData(const QByteArray &data) { m_buffer.append(data); }
    QByteArray data() const { return m_buffer; }

private:
    int m_id;
    int m_cacheId;
    QString m_imageName;
    QByteArray m_buffer;
    QPointer<QObject> m_callbackObject;
    QString m_callbackMethod;
    QSize m_scalingSize;
};

#endif // IMAGECACHE_H
