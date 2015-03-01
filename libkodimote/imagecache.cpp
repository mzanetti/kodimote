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
#include <QtConcurrent/QtConcurrent>

ImageFetchJob *scaleImage(ImageFetchJob *job, QByteArray data)
{
    QString cachedFile = job->cachedFile();
    QFileInfo fi = cachedFile;

    if(job->scaleTo().width() > 0 && job->scaleTo().height() > 0) {
        if(fi.suffix() == "png" || fi.suffix() == "jpg") {
            QImage image = QImage::fromData(data);
            QImage scaledImage = image.scaled(job->scaleTo().width(), job->scaleTo().height(), Qt::KeepAspectRatio, Qt::FastTransformation);
            scaledImage.save(cachedFile);
        } else {
            QFile file(cachedFile);
            if(file.open(QIODevice::WriteOnly)) {
                file.write(data);
            }
        }
    }

    return job;
}

KodiImageCache::KodiImageCache(QObject *parent) :
    QObject(parent),
    m_jobId(0),
    m_doubleDecode(false)
{
}

bool KodiImageCache::contains(const QString &image, int cacheId, QString &cachedFile)
{
    QString cacheKey = this->cacheKey(image, cacheId);
    if (!m_cacheFiles.contains(cacheKey)) {
        QString path = cachePath(cacheId);
        // Make sure the cache exists
        QDir dir(path);
        if(!dir.exists()) {
            dir.mkpath(path);
        }

        cachedFile = KodiImageCache::cachedFile(path, image);
        QFileInfo fi(cachedFile);
        m_cacheFiles.insert(cacheKey, QPair<bool, QString>(fi.exists(), cachedFile));
    } else {
        cachedFile = m_cacheFiles[cacheKey].second;
    }

    return m_cacheFiles[cacheKey].first;
}

QString KodiImageCache::cachedFile(const QString &path, const QString &image)
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
    return path + url.path();
}

QString KodiImageCache::cachePath(int cacheId)
{
    return Kodi::instance()->dataPath() + "/imagecache/" + QString::number(cacheId) + "/";
}

int KodiImageCache::fetch(const QString &image, QObject *callbackObject, const QString &callbackFunction, const QSize &scaleTo, int cacheId)
{
    QString cacheKey = this->cacheKey(image, cacheId);
    ImageFetchJob *job = m_jobs.value(cacheKey);
    if (job) {
        foreach (const ImageFetchJob::Callback callback, job->callbacks()) {
            if (callback.object() == callbackObject && callback.method() == callbackFunction) {
                return job->id();
            }
        }

        job->appendCallback(QPointer<QObject>(callbackObject), callbackFunction);
        return job->id();
    }

    QString cachedFile = m_cacheFiles.contains(cacheKey) ?
                m_cacheFiles[cacheKey].second :
                KodiImageCache::cachedFile(cachePath(cacheId), image);

    // Ok... this is a new one... start fetching it
    ImageFetchJob *ifJob = new ImageFetchJob(m_jobId++, cacheId, image, cachedFile, scaleTo);
    ifJob->appendCallback(QPointer<QObject>(callbackObject), callbackFunction);
    m_jobs.insert(cacheKey, ifJob);
    fetchNext(ifJob);

    return ifJob->id();
}

QString KodiImageCache::cacheKey(const QString &image, int cacheId)
{
    return image + "-" + QString::number(cacheId);
}

void KodiImageCache::imageFetched()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    ImageFetchJob *job = static_cast<ImageFetchJob*>(reply->request().originatingObject());
    QString cacheKey = this->cacheKey(job->imageName(), job->cacheId());

    reply->request().setOriginatingObject(0);
    reply->deleteLater();

    if(reply->error() == QNetworkReply::NoError) {
        QFutureWatcher<ImageFetchJob*> *watcher = new QFutureWatcher<ImageFetchJob*>();
        connect(watcher, SIGNAL(finished()), this, SLOT(imageScaled()));
        watcher->setFuture(QtConcurrent::run(scaleImage, job, reply->readAll()));
        return;
    }

    qDebug() << "image fetching failed" << reply->errorString() << reply->error();

    // Hack: There is something fishy with Kodi's image urls. Some versions require decoding
    // from PercentageDecoding once, some twice... Let's retry doubleDecoding if if we get a 404
    if (reply->error() == QNetworkReply::ContentNotFoundError) {
        if (!m_doubleDecode) {
            m_doubleDecode = true;
            fetchNext(job);
            return;
        } else {
            // So this failed with both... Lets turn off doubleDecode again and try the next
            m_doubleDecode = false;
        }
    }

    foreach (const ImageFetchJob::Callback callback, job->callbacks()) {
        QMetaObject::invokeMethod(callback.object().data(), callback.method().toLatin1(), Qt::QueuedConnection, Q_ARG(int, job->id()));
    }

    job->deleteLater();
    m_cacheFiles.insert(cacheKey, qMakePair<bool, QString>(true, ""));
    m_jobs.remove(cacheKey);
}

void KodiImageCache::imageScaled()
{
    QFutureWatcher<ImageFetchJob*> *watcher = static_cast<QFutureWatcher<ImageFetchJob*>*>(QObject::sender());
    watcher->deleteLater();
    ImageFetchJob *job = watcher->result();
    QString cachedFile = job->cachedFile();
    QString cacheKey = this->cacheKey(job->imageName(), job->cacheId());

    m_cacheFiles.insert(cacheKey, QPair<bool, QString>(true, cachedFile));

    foreach (const ImageFetchJob::Callback callback, job->callbacks()) {
        QMetaObject::invokeMethod(callback.object().data(), callback.method().toLatin1(), Qt::QueuedConnection, Q_ARG(int, job->id()));
    }

    job->deleteLater();
}

void KodiImageCache::fetchNext(ImageFetchJob *job)
{
    QString cacheKey = this->cacheKey(job->imageName(), job->cacheId());
    QFileInfo fi(job->cachedFile());
    QDir dir(fi.absolutePath());
    if(!(dir.exists() || dir.mkpath(fi.absolutePath()))) {
        qDebug() << "cannot open file." << job->cachedFile() << "won't fetch artwork";
        m_jobs.remove(cacheKey);
        job->deleteLater();
        return;
    }

    QVariantMap params;
    params.insert("path", job->imageName());
    int id = KodiConnection::sendParallelCommand("Files.PrepareDownload", params, this, "downloadPrepared");
    m_fetchQueue.insert(id, cacheKey);
}

void KodiImageCache::downloadPrepared(const QVariantMap &rsp)
{
    QVariantMap result = rsp.value("result").toMap();
    KodiHost *host = KodiConnection::connectedHost();

    if(host == 0 || result.isEmpty()) {
        return;
    }

    int id = rsp.value("id").toInt();
    QString cacheKey = m_fetchQueue[id];
    ImageFetchJob *job = m_jobs[cacheKey];

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
    imageRequest.setOriginatingObject(job);
    QNetworkReply *reply = KodiConnection::nam()->get(imageRequest);
    connect(reply, SIGNAL(finished()), SLOT(imageFetched()));
}
