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

#include "imagecache.h"
#include "kodi.h"
#include "kodiconnection.h"
#include "kodihostmodel.h"

#include <QImage>
#include <QFileInfo>
#include <QDir>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

KodiImageCache::KodiImageCache(QObject *parent) :
    QThread(parent),
    m_jobId(0),
    m_currentJob(0),
    m_doubleDecode(false)
{
    m_fetchNextTimer = new QTimer(this);
    m_fetchNextTimer->setSingleShot(true);
    m_fetchNextTimer->setInterval(0);
    connect(m_fetchNextTimer,SIGNAL(timeout()),this,SLOT(fetchNext()));
}

bool KodiImageCache::contains(const QString &image, int cacheId)
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
        qDebug() << "checking from file:" << fi.path() << fi.exists();
    }

    return m_cacheFiles.at(cacheId).value(image);
}

QString KodiImageCache::cachedFile(const QString &image, int cacheId)
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
    QUrl url = QUrl::fromPercentEncoding(filename.toUtf8());
    return cachePath(cacheId) + url.path();
}

QString KodiImageCache::cachePath(int cacheId)
{
    return Kodi::instance()->dataPath() + "/imagecache/" + QString::number(cacheId) + "/";
}

void KodiImageCache::run()
{
    exec();
}

int KodiImageCache::fetch(const QString &image, QObject *callbackObject, const QString &callbackFunction, const QSize &scaleTo, int cacheId)
{
    QString cacheKey = this->cacheKey(image, cacheId);
    QMutexLocker locker(&m_mutex);
    ImageFetchJob *job = m_jobs.value(cacheKey);
    if (job) {
        if (m_downloadQueue.contains(cacheKey)) {
            m_downloadQueue.move(m_downloadQueue.indexOf(cacheKey), 0);
        }

        foreach (const ImageFetchJob::Callback callback, job->callbacks()) {
            if (callback.object() == callbackObject && callback.method() == callbackFunction) {
                return job->id();
            }
        }

        job->appendCallback(QPointer<QObject>(callbackObject), callbackFunction);
        return job->id();
    }

    // Ok... this is a new one... start fetching it
    ImageFetchJob *ifJob = new ImageFetchJob(m_jobId++, cacheId, image, cachedFile(image, cacheId), scaleTo);
    ifJob->appendCallback(QPointer<QObject>(callbackObject), callbackFunction);
    m_jobs.insert(cacheKey, ifJob);
    m_downloadQueue.prepend(cacheKey);
    m_fetchNextTimer->start();

    return ifJob->id();
}

QString KodiImageCache::cacheKey(const QString &image, int cacheId)
{
    return image + "-" + QString::number(cacheId);
}

void KodiImageCache::imageFetched()
{
    QMutexLocker locker(&m_mutex);

    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    ImageFetchJob *job = static_cast<ImageFetchJob*>(reply->request().originatingObject());
    QString cacheKey = this->cacheKey(job->imageName(), job->cacheId());

    if(reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();

        QString cachedFile = job->cachedFile();
        QFileInfo fi = cachedFile;

        if(job->scaleTo().width() > 0 && job->scaleTo().height() > 0) {
            if(fi.suffix() == "png" || fi.suffix() == "jpg") {
                QImage image = QImage::fromData(data);
                qDebug() << "scaling image" << fi.path() << fi.fileName();
                QImage scaledImage = image.scaled(job->scaleTo().width(), job->scaleTo().height(), Qt::KeepAspectRatio, Qt::FastTransformation);
                scaledImage.save(cachedFile);
            } else {
                qDebug() << "NOT scaling image" << fi.fileName();
                QFile file(cachedFile);
                if(file.open(QIODevice::WriteOnly)) {
                    file.write(data);
                }
            }
        }

        m_cacheFiles[job->cacheId()].insert(job->imageName(), true);
        qDebug() << "fetched, notifying callbacks:" << job->callbacks().size();
        foreach (const ImageFetchJob::Callback callback, job->callbacks()) {
            QMetaObject::invokeMethod(callback.object().data(), callback.method().toLatin1(), Qt::QueuedConnection, Q_ARG(int, job->id()));
        }
    } else {
        qDebug() << "image fetching failed" << reply->errorString() << reply->error();

        // Hack: There is something fishy with Kodi's image urls. Some versions require decoding
        // from PercentageDecoding once, some twice... Let's retry doubleDecoding if if we get a 404
        if (reply->error() == QNetworkReply::ContentNotFoundError) {
            if (!m_doubleDecode) {
                m_doubleDecode = true;
                m_downloadQueue.prepend(cacheKey);
                m_fetchNextTimer->start();
                return;
            } else {
                // So this failed with both... Lets turn off doubleDecode again and try the next
                m_doubleDecode = false;
            }
        }

        QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        qDebug() << possibleRedirectUrl;

        if(!possibleRedirectUrl.toString().isEmpty() && possibleRedirectUrl != reply->url()) {
            qDebug() << "We got redirected to" << possibleRedirectUrl;
        }
    }

    m_jobs.remove(cacheKey);
    job->deleteLater();
    reply->deleteLater();
}

void KodiImageCache::fetchNext()
{
    m_fetchNextTimer->start();
    if(m_currentJob != 0 || m_downloadQueue.isEmpty()) {
        return;
    }

    m_currentJob = m_jobs[m_downloadQueue.takeFirst()];
    QFileInfo fi(cachedFile(m_currentJob->imageName(), m_currentJob->cacheId()));
    QDir dir(fi.absolutePath());
    if(!(dir.exists() || dir.mkpath(fi.absolutePath()))) {
        qDebug() << "cannot open file." << cachedFile(m_currentJob->imageName(), m_currentJob->cacheId()) << "won't fetch artwork";
        cleanupAndTriggerNext();
        return;
    }

    QVariantMap params;
    params.insert("path", m_currentJob->imageName());
    KodiConnection::sendCommand("Files.PrepareDownload", params, this, "downloadPrepared");
}

void KodiImageCache::downloadPrepared(const QVariantMap &rsp)
{

    QVariantMap result = rsp.value("result").toMap();
    KodiHost *host = KodiConnection::connectedHost();

    if(host == 0 || result.isEmpty()) {
        qDebug() << "couldn't prepare download:" << m_currentJob->imageName();
        cleanupAndTriggerNext();
        return;
    }

    QUrl imageUrl;

#ifdef QT5_BUILD
    if (m_doubleDecode) {
        QByteArray path = "/" + QByteArray::fromPercentEncoding(result.value("details").toMap().value("path").toByteArray());
        imageUrl = QUrl::fromEncoded(path);
    } else {
        imageUrl = QUrl::fromEncoded("/" + result.value("details").toMap().value("path").toByteArray());
    }
#else
    QString path = "/" + QUrl::fromPercentEncoding(result.value("details").toMap().value("path").toByteArray());
    imageUrl.setPath(path);
#endif

    imageUrl.setScheme(result.value("protocol").toString());
    imageUrl.setHost(host->address());
    imageUrl.setPort(host->port());

    QNetworkRequest imageRequest(imageUrl);
    imageRequest.setOriginatingObject(m_currentJob);
    QNetworkReply *reply = KodiConnection::nam()->get(imageRequest);
    connect(reply, SIGNAL(finished()), SLOT(imageFetched()));

    QMutexLocker locker(&m_mutex);
    m_currentJob = 0;
    locker.unlock();
    m_fetchNextTimer->start();
}

void KodiImageCache::cleanupAndTriggerNext()
{
    m_jobs.remove(cacheKey(m_currentJob->imageName(), m_currentJob->cacheId()));
    m_currentJob->deleteLater();
    m_currentJob = 0;
    m_fetchNextTimer->start();
}
