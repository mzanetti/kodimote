#include "imagecache.h"
#include "xbmc.h"
#include "xbmcconnection.h"

#include <QGraphicsObject>
#include <QFileInfo>
#include <QDir>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

ImageCache::ImageCache(QObject *parent) :
    QThread(parent),
    m_jobId(0),
    m_currentJob(0)
{
    QDir dir(cachePath());
    if(!dir.exists()) {
        dir.mkpath(cachePath());
    }
}

bool ImageCache::contains(const QString &image)
{
    QMutexLocker locker(&m_mutex);
    if(!m_cacheFiles.contains(image)) {
        qDebug() << "checking from file";
        QUrl url = QUrl::fromPercentEncoding(image.toLocal8Bit());
        QFileInfo fi(cachePath() + url.path());
        m_cacheFiles.insert(image, fi.exists());
    }

    return m_cacheFiles.value(image);
}

QString ImageCache::cachedFile(const QString &image)
{
    QString filename = image;
    if(filename.endsWith(".tbn")) {
        filename.replace(".tbn", ".jpg");
    }
    QUrl url = QUrl::fromPercentEncoding(filename.toLocal8Bit());
    return cachePath() + url.path();
}

QString ImageCache::cachePath()
{
    return QDir::home().absolutePath() + "/.xbmcremote/imagecache/";
}

void ImageCache::run()
{
    exec();
}

int ImageCache::fetch(const QString &image, QObject *callbackObject, const QString &callbackFunction)
{
    ImageFetchJob *ifJob = new ImageFetchJob(m_jobId++, image, QPointer<QObject>(callbackObject), callbackFunction);

    QMutexLocker locker(&m_mutex);
    if(m_currentJob && m_currentJob->imageName() == image) {
        qDebug() << "already fetching image queued for" << image;
        m_toBeNotifiedList.append(ifJob);
        return ifJob->id();
    }
    foreach(ImageFetchJob *job, m_downloadQueue) {
        if(job->imageName() == image) {
            qDebug() << "image fetching already queued for" << image;
            // move it to the beginning of the queue to speed up user feedback
            m_downloadQueue.prepend(m_downloadQueue.takeAt(m_downloadQueue.indexOf(job)));
            m_toBeNotifiedList.append(ifJob);
            return ifJob->id();
        }
    }

    // Ok... this is a new one... start fetching it
    m_downloadQueue.prepend(ifJob);
    QMetaObject::invokeMethod(this, "fetchNext", Qt::QueuedConnection);

    return ifJob->id();
}

void ImageCache::imageFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    QMutexLocker locker(&m_mutex);

    if(reply->error() == QNetworkReply::NoError) {

        m_currentJob->appendData(reply->readAll());

        QImage image = QImage::fromData(m_currentJob->data());

        QFileInfo fi = cachedFile(m_currentJob->imageName());

        if(fi.suffix() == "png" || fi.suffix() == "jpg") {
            // Those values are really optimized for the Harmattan theme...
            qDebug() << "scaling image" << fi.fileName();
            QImage scaledImage = image.scaled(152, 120, Qt::KeepAspectRatio, Qt::FastTransformation);
            scaledImage.save(cachedFile(m_currentJob->imageName()));
        } else {
            qDebug() << "NOT scaling image" << fi.fileName();
            QFile file(cachedFile(m_currentJob->imageName()));
            if(file.open(QIODevice::WriteOnly)) {
                file.write(m_currentJob->data());
            }
        }

        m_cacheFiles.insert(m_currentJob->imageName(), true);

        // Notify original requester
        QMetaObject::invokeMethod(m_currentJob->callbackObject().data(), m_currentJob->callbackMethod().toAscii(), Qt::QueuedConnection, Q_ARG(int, m_currentJob->id()));

        // Notify all duplicate requests
        QList<ImageFetchJob*> newList;
        foreach(ImageFetchJob* job, m_toBeNotifiedList) {
            if(job->imageName() == m_currentJob->imageName()) {
                newList.append(job);
            }
        }
        qDebug() << "found" << newList.count() << "duplicate requests for" << m_currentJob->id();
        while(!newList.isEmpty()) {
            ImageFetchJob *job = newList.takeFirst();
            m_toBeNotifiedList.removeAll(job);
            QMetaObject::invokeMethod(job->callbackObject().data(), job->callbackMethod().toAscii(), Qt::QueuedConnection, Q_ARG(int, job->id()));
            delete job;
        }
    } else {
        qDebug() << "image fetching failed" << reply->errorString();
    }


    delete m_currentJob;
    m_currentJob = 0;

    reply->deleteLater();

    QMetaObject::invokeMethod(this, "fetchNext", Qt::QueuedConnection);
}

void ImageCache::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived)
    Q_UNUSED(bytesTotal)

    QMutexLocker locker(&m_mutex);
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    m_currentJob->appendData(reply->readAll());
}

void ImageCache::fetchNext()
{

    if(m_currentJob != 0 || m_downloadQueue.isEmpty()) {
        return;
    }

    m_currentJob = m_downloadQueue.takeFirst();
    QFileInfo fi(cachedFile(m_currentJob->imageName()));
    QDir dir(fi.absolutePath());
    if(!(dir.exists() || dir.mkpath(fi.absolutePath()))) {
        qDebug() << "cannot open file." << cachedFile(m_currentJob->imageName()) << "won't fetch artwork";
        delete m_currentJob;
        m_currentJob = 0;
        QMetaObject::invokeMethod(this, "fetchNext", Qt::QueuedConnection);
        return;
    }

    QNetworkRequest imageRequest(Xbmc::instance()->vfsPath() + m_currentJob->imageName());
    QNetworkReply *reply = XbmcConnection::nam()->get(imageRequest);
    connect(reply, SIGNAL(finished()), SLOT(imageFetched()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
}

