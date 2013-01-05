#include "xbmcdownload.h"

XbmcDownload::XbmcDownload(QObject *parent) :
    QObject(parent),
    m_cancelled(false)
{
}

void XbmcDownload::setSource(const QString &source)
{
    m_source = source;
    emit sourceChanged();
}

QString XbmcDownload::source() const
{
    return m_source;
}

void XbmcDownload::setDestination(const QString &destination)
{
    m_destination = destination;
    emit destinationChanged();
}

QString XbmcDownload::destination() const
{
    return m_destination;
}

void XbmcDownload::setFile(QFile *file)
{
    m_file = file;
}

QFile *XbmcDownload::file() const
{
    return m_file;
}

void XbmcDownload::setTotal(qint64 total)
{
    m_total = total;
    emit totalChanged();
}

qint64 XbmcDownload::total() const
{
    return m_total;
}

void XbmcDownload::setProgress(qint64 progress)
{
    m_progress = progress;
    emit progressChanged();
}

qint64 XbmcDownload::progress() const
{
    return m_progress;
}

void XbmcDownload::setStarted()
{
    emit started();
}

void XbmcDownload::setFinished(bool success)
{
    emit finished(success);
}

void XbmcDownload::setIconId(const QString &iconId)
{
    m_icon = iconId;
}

QString XbmcDownload::iconId() const
{
    return m_icon;
}

void XbmcDownload::setLabel(const QString &label)
{
    m_label = label;
}

QString XbmcDownload::label() const
{
    return m_label;
}

bool XbmcDownload::isCancelled() const
{
    return m_cancelled;
}

void XbmcDownload::cancel()
{
    emit cancelled();
    m_cancelled = true;
}
