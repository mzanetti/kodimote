#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

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
    void openSettingsDialog();
    void openAboutDialog();

private:
    void grabZoomKeys(bool grab);

    QmlApplicationViewer *viewer;
    Settings *m_settings;

};

#endif // MAINWINDOW_H
