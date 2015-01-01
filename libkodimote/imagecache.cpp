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

    QMutexLocker locker(&m_mutex);
    if(m_currentJob && m_currentJob->imageName() == image && scaleTo == m_currentJob->scaleTo()) {
        qDebug() << "already fetching image queued for" << image;

        if (m_currentJob->callbackObject() == callbackObject && m_currentJob->callbackMethod() == callbackFunction) {
            return m_currentJob->id();
        }

        ImageFetchJob *ifJob = new ImageFetchJob(m_jobId++, cacheId, image, QPointer<QObject>(callbackObject), callbackFunction, scaleTo);
        m_toBeNotifiedList.append(ifJob);
        qDebug() << m_toBeNotifiedList.count();
        return ifJob->id();
    }
    foreach(ImageFetchJob *job, m_downloadQueue) {
        if(job->imageName() == image && job->scaleTo() == scaleTo) {
            qDebug() << "image fetching already queued for" << image;

            if (job->callbackObject() == callbackObject && job->callbackMethod() == callbackFunction) {
                // move it to the beginning of the queue to speed up user feedback
                m_downloadQueue.move(m_downloadQueue.indexOf(job), 0);
                return job->id();
            }
            ImageFetchJob *ifJob = new ImageFetchJob(m_jobId++, cacheId, image, QPointer<QObject>(callbackObject), callbackFunction, scaleTo);
            // prepend to speed up user feedback
            m_downloadQueue.prepend(m_downloadQueue.takeAt(m_downloadQueue.indexOf(job)));
            m_toBeNotifiedList.append(ifJob);
            qDebug() << m_toBeNotifiedList.count();
            return ifJob->id();
        }
    }

    // Ok... this is a new one... start fetching it
    ImageFetchJob *ifJob = new ImageFetchJob(m_jobId++, cacheId, image, QPointer<QObject>(callbackObject), callbackFunction, scaleTo);
    m_downloadQueue.prepend(ifJob);
    m_fetchNextTimer->start();

    return ifJob->id();
}

void KodiImageCache::imageFetched()
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
        qDebug() << "image fetching failed" << reply->errorString() << reply->error();

        // Hack: There is something fishy with Kodi's image urls. Some versions require decoding
        // from PercentageDecoding once, some twice... Let's retry doubleDecoding if if we get a 404
        if (!m_doubleDecode && reply->error() == QNetworkReply::ContentNotFoundError) {
            m_doubleDecode = true;
            m_downloadQueue.prepend(m_currentJob);
            m_currentJob = 0;
            m_fetchNextTimer->start();
            return;
        } else if (m_doubleDecode && reply->error() == QNetworkReply::ContentNotFoundError) {
            // So this failed with both... Lets turn off doubleDecode again and try the next
            m_doubleDecode = false;
        }

        QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        qDebug() << possibleRedirectUrl;

        if(!possibleRedirectUrl.toString().isEmpty() && possibleRedirectUrl != reply->url()) {
            qDebug() << "We got redirected to" << possibleRedirectUrl;
        }
    }


    delete m_currentJob;
    m_currentJob = 0;

    reply->deleteLater();

    m_fetchNextTimer->start();
}

void KodiImageCache::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived)
    Q_UNUSED(bytesTotal)

    QMutexLocker locker(&m_mutex);
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    m_currentJob->appendData(reply->readAll());
}

void KodiImageCache::fetchNext()
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

    qDebug() << "fetching image:" << imageUrl;

    QNetworkRequest imageRequest(imageUrl);
    QNetworkReply *reply = KodiConnection::nam()->get(imageRequest);
    connect(reply, SIGNAL(finished()), SLOT(imageFetched()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
}

void KodiImageCache::cleanupAndTriggerNext()
{
    delete m_currentJob;
    m_currentJob = 0;
    m_fetchNextTimer->start();
}
