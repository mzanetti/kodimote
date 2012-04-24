#ifndef XBMCDOWNLOAD_H
#define XBMCDOWNLOAD_H

#include <QObject>
#include <QFile>

class XbmcDownload : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString destination READ destination WRITE setDestination NOTIFY destinationChanged)
    Q_PROPERTY(qint64 total READ total WRITE setTotal NOTIFY totalChanged)
    Q_PROPERTY(qint64 progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QString iconId READ iconId WRITE setIconId)
    Q_PROPERTY(QString label READ label WRITE setLabel)

public:
    explicit XbmcDownload(QObject *parent = 0);
    
    void setSource(const QString &source);
    QString source() const;

    void setDestination(const QString &destination);
    QString destination() const;

    void setFile(QFile *file);
    QFile *file() const;

    void setTotal(qint64 total);
    qint64 total() const;

    void setProgress(qint64 progress);
    qint64 progress() const;

    void setStarted();
    void setFinished(bool success);

    void setIconId(const QString &iconId);
    QString iconId() const;

    void setLabel(const QString &label);
    QString label() const;

    bool isCancelled() const;

signals:
    void sourceChanged();
    void destinationChanged();
    void totalChanged();
    void progressChanged();
    void started();
    void finished(bool success);

    void cancelled();
    
public slots:
    void cancel();

private:
    QString m_source;
    QString m_destination;
    QFile *m_file;
    qint64 m_total;
    qint64 m_progress;
    QString m_icon;
    QString m_label;
    bool m_cancelled;
    
};

#endif // XBMCDOWNLOAD_H
