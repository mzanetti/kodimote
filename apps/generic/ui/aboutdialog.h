#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);

signals:

private slots:
    void openDonationPage();
    void openFlattrPage();

};

#endif // ABOUTDIALOG_H
