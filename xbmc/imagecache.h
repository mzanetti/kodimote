#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include <QObject>
#include <QHash>
#include <QPointer>
#include <QBuffer>

class QNetworkReply;

class ImageFetchJob;
class QFile;

class ImageCache : public QObject
{
    Q_OBJECT
public:
    explicit ImageCache(QObject *parent = 0);
    
    bool contains(const QString &image);
    QString cachedFile(const QString &image);

    static QString cachePath();

public slots:
    int fetch(const QString &image, QObject *callbackObject, const QString &callbackFunction);

private slots:
    void imageFetched();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void fetchNext();

private:
    int m_jobId;

    ImageFetchJob *m_currentJob;
    QList<ImageFetchJob*> m_downloadQueue;
    QList<ImageFetchJob*> m_toBeNotifiedList;

    QHash<QString, bool> m_cacheFiles;

};

class ImageFetchJob
{
public:
    ImageFetchJob(int id, const QString &imageName, QPointer<QObject> callbackObject, const QString &callbackMethod) :
        m_id(id),
        m_imageName(imageName),
        m_callbackObject(callbackObject),
        m_callbackMethod(callbackMethod)
    {
    }
    ~ImageFetchJob()
    {
    }

    int id() { return m_id; }
    QString imageName() const { return m_imageName; }
    QPointer<QObject> callbackObject() const { return m_callbackObject; }
    QString callbackMethod() const { return m_callbackMethod; }

    void appendData(const QByteArray &data) { m_buffer.append(data); }
    QByteArray data() const { return m_buffer; }

private:
    int m_id;
    QString m_imageName;
    QByteArray m_buffer;
    QPointer<QObject> m_callbackObject;
    QString m_callbackMethod;
};

#endif // IMAGECACHE_H
