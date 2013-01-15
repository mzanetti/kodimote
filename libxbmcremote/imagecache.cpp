#include "imagecache.h"
#include "xbmc.h"
#include "xbmcconnection.h"
#include "xbmchostmodel.h"

#include <QImage>
#include <QFileInfo>
#include <QDir>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

XbmcImageCache::XbmcImageCache(QObject *parent) :
    QThread(parent),
    m_jobId(0),
    m_currentJob(0)
{
}

bool XbmcImageCache::contains(const QString &image, int cacheId)
{
    QMutexLocker locker(&m_mutex);

    // Make sure the cache exists
    QDir dir(cachePath(cacheId));
    if(!dir.exists()) {
        dir.mkpath(cachePath(cacheId));
    }

    while(m_cacheFiles.count() <= cacheId) {
        m_cacheFiles.append(QHash<QString, bool>());
    }

    if(!m_cacheFiles.at(cacheId).contains(image)) {
        QFileInfo fi(cachedFile(image, cacheId));
        m_cacheFiles[cacheId].insert(image, fi.exists());
//        qDebug() << "checking from file:" << fi.path() << fi.exists();
    }

    return m_cacheFiles.at(cacheId).value(image);
}

QString XbmcImageCache::cachedFile(const QString &image, int cacheId)
{
    QString filename = image;
    if(filename.endsWith("/")) {
        filename = filename.left(filename.length() - 1);
    }
    if(filename.endsWith(".tbn")) {
        filename.replace(".tbn", ".jpg");
    }
    if(filename.endsWith(".mp3")) {
        filename.replace(".mp3", ".jpg");
    }
    QUrl url = QUrl::fromPercentEncoding(filename.toLocal8Bit());
    return cachePath(cacheId) + url.path();
}

QString XbmcImageCache::cachePath(int cacheId)
{
    return QDir::home().absolutePath() + "/.xbmcremote/imagecache/" + QString::number(cacheId) + "/";
}

void XbmcImageCache::run()
{
    exec();
}

int XbmcImageCache::fetch(const QString &image, QObject *callbackObject, const QString &callbackFunction, const QSize &scaleTo, int cacheId)
{
    ImageFetchJob *ifJob = new ImageFetchJob(m_jobId++, cacheId, image, QPointer<QObject>(callbackObject), callbackFunction, scaleTo);

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

void XbmcImageCache::imageFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    QMutexLocker locker(&m_mutex);

    if(reply->error() == QNetworkReply::NoError) {

        m_currentJob->appendData(reply->readAll());

        QImage image = QImage::fromData(m_currentJob->data());

        QFileInfo fi = cachedFile(m_currentJob->imageName(), m_currentJob->cacheId());

        if(m_currentJob->scaleTo().width() > 0 && m_currentJob->scaleTo().height() > 0) {
            if(fi.suffix() == "png" || fi.suffix() == "jpg") {
                qDebug() << "scaling image" << fi.path() << fi.fileName();
                QImage scaledImage = image.scaled(m_currentJob->scaleTo().width(), m_currentJob->scaleTo().height(), Qt::KeepAspectRatio, Qt::FastTransformation);
                scaledImage.save(cachedFile(m_currentJob->imageName(), m_currentJob->cacheId()));
            } else {
                qDebug() << "NOT scaling image" << fi.fileName();
                QFile file(cachedFile(m_currentJob->imageName(), m_currentJob->cacheId()));
                if(file.open(QIODevice::WriteOnly)) {
                    file.write(m_currentJob->data());
                }
            }
        }

        m_cacheFiles[m_currentJob->cacheId()].insert(m_currentJob->imageName(), true);

        // Notify original requester
        QMetaObject::invokeMethod(m_currentJob->callbackObject().data(), m_currentJob->callbackMethod().toLatin1(), Qt::QueuedConnection, Q_ARG(int, m_currentJob->id()));

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
            QMetaObject::invokeMethod(job->callbackObject().data(), job->callbackMethod().toLatin1(), Qt::QueuedConnection, Q_ARG(int, job->id()));
            delete job;
        }
    } else {
        qDebug() << "image fetching failed" << reply->errorString();
        QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        qDebug() << possibleRedirectUrl;

        if(!possibleRedirectUrl.toString().isEmpty() && possibleRedirectUrl != reply->url()) {
            qDebug() << "We got redirected to" << possibleRedirectUrl;
        }
    }


    delete m_currentJob;
    m_currentJob = 0;

    reply->deleteLater();

    QMetaObject::invokeMethod(this, "fetchNext", Qt::QueuedConnection);
}

void XbmcImageCache::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived)
    Q_UNUSED(bytesTotal)

    QMutexLocker locker(&m_mutex);
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    m_currentJob->appendData(reply->readAll());
}

void XbmcImageCache::fetchNext()
{

    if(m_currentJob != 0 || m_downloadQueue.isEmpty()) {
        return;
    }

    m_currentJob = m_downloadQueue.takeFirst();
    QFileInfo fi(cachedFile(m_currentJob->imageName(), m_currentJob->cacheId()));
    QDir dir(fi.absolutePath());
    if(!(dir.exists() || dir.mkpath(fi.absolutePath()))) {
        qDebug() << "cannot open file." << cachedFile(m_currentJob->imageName(), m_currentJob->cacheId()) << "won't fetch artwork";
        cleanupAndTriggerNext();
        return;
    }

    QVariantMap params;
    params.insert("path", m_currentJob->imageName());
    XbmcConnection::sendCommand("Files.PrepareDownload", params, this, "downloadPrepared");
}

void XbmcImageCache::downloadPrepared(const QVariantMap &rsp)
{

    QVariantMap result = rsp.value("result").toMap();
    XbmcHost *host = XbmcConnection::connectedHost();

    if(host == 0 || result.isEmpty()) {
        qDebug() << "couldn't prepare download:" << m_currentJob->imageName();
        cleanupAndTriggerNext();
        return;
    }

    QUrl imageUrl;
    imageUrl.setScheme(result.value("protocol").toString());
    imageUrl.setHost(host->address());
    imageUrl.setPort(host->port());

    QByteArray path = "/" + QByteArray::fromPercentEncoding(result.value("details").toMap().value("path").toByteArray());

#ifdef QT5_BUILD
    imageUrl.setPath(path, QUrl::DecodedMode);
#else
    imageUrl.setPath(path);
#endif

    QNetworkRequest imageRequest(imageUrl);
    QNetworkReply *reply = XbmcConnection::nam()->get(imageRequest);
    connect(reply, SIGNAL(finished()), SLOT(imageFetched()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
}

void XbmcImageCache::cleanupAndTriggerNext()
{
    delete m_currentJob;
    m_currentJob = 0;
    QMetaObject::invokeMethod(this, "fetchNext", Qt::QueuedConnection);
}
