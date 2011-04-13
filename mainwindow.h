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

    Q_PROPERTY(QString state READ state NOTIFY orientationChanged)

public:
    MainWindow();

    QString state() const;

signals:
    void orientationChanged();

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void openSettings();
    void myslot();

private:
    QDeclarativeView *m_viewer;
    Xbmc::Xbmc *xbmc;


    void setMainQmlFile(const QString &file);
    QString adjustPath(const QString &path);
};

#endif // MAINWINDOW_H
