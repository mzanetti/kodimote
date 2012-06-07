#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#ifdef Q_WS_MAEMO_5
#include <QtDBus/QDBusObjectPath>
#include <QtMaemo5/QMaemo5InformationBox>
#endif
#include <xbmc/xbmcconnection.h>

class Settings;
class QmlApplicationViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(Settings *settings, QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);

private slots:
    void orientationChanged();
    void openSettingsDialog();
    void openConnectDialog();
    void openAboutDialog();

#ifdef Q_WS_MAEMO_5
    void grabZoomKeys(bool grab);
    void callEvent(const QDBusObjectPath &, const QString &);
    void callTerminated();
    void downloadAdded(XbmcDownload* download);
    void downloadStarted();
    void downloadDone(bool success);
#endif

    void authenticationRequired(const QString &hostname, const QString &address);
    void connectionChanged(bool connected);
    void hostRemoved();


private:
    QmlApplicationViewer *viewer;
    Settings *m_settings;

    bool m_videoPaused;
    bool m_audioPaused;

    QMaemo5InformationBox m_infoBox;

};

#endif // MAINWINDOW_H
