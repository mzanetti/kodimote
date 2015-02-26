#ifndef MPRISAPPLICATION_H
#define MPRISAPPLICATION_H

#include <QtDBus/QDBusAbstractAdaptor>

#include "protocolhandlers/protocolmanager.h"

class MprisApplication : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_PROPERTY(bool CanQuit READ canQuit)
    Q_PROPERTY(bool CanSetFullscreen READ canSetFullscreen)
    Q_PROPERTY(bool CanRaise READ canRaise)
    Q_PROPERTY(bool HasTrackList READ hasTrackList)
    Q_PROPERTY(QString Identity READ identity)
    Q_PROPERTY(QString DesktopEntry READ desktopEntry)
    Q_PROPERTY(QStringList SupportedUriSchemes READ supportedUriSchemes)
    Q_PROPERTY(QStringList SupportedMimeTypes READ supportedMimeTypes)
    Q_PROPERTY(bool Fullscreen READ fullscreen WRITE setFullscreen)
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
public:
    explicit MprisApplication(ProtocolManager *protocols, QObject *parent = 0);

    bool canQuit() const;
    bool canSetFullscreen() const;
    bool canRaise() const;
    bool hasTrackList() const;
    QString identity() const;
    QString desktopEntry() const;
    QStringList supportedUriSchemes() const;
    QStringList supportedMimeTypes() const;

    bool fullscreen() const;
    void setFullscreen(bool fullscreen);

public slots:
    void Quit();
    void Raise();

private:
    ProtocolManager *m_protocols;
};

#endif // MPRISAPPLICATION_H
