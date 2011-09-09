#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QmlApplicationViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

private slots:
    void openSettingsDialog();
    void openAboutDialog();

private:
    QmlApplicationViewer *viewer;

};

#endif // MAINWINDOW_H
