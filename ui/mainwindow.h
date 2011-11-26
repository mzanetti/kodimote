#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#ifdef Q_WS_MAEMO_5
#include <QtDBus/QDBusObjectPath>
#endif
#include <xbmc/xbmcconnection.h>

class Settings;
class QmlApplicationViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
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
#endif

private:

    QmlApplicationViewer *viewer;
    Settings *m_settings;

};

#endif // MAINWINDOW_H
