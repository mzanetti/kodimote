#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmlapplicationviewer.h"

#include <QMainWindow>

namespace Xbmc
{
class Xbmc;
}

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();


private slots:
    void openSettings();

private:
    QDeclarativeView *m_viewer;
    Xbmc::Xbmc *xbmc;


    void setMainQmlFile(const QString &file);
    QString adjustPath(const QString &path);
};

#endif // MAINWINDOW_H
