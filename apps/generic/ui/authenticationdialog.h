#ifndef AUTHENTICATIONDIALOG_H
#define AUTHENTICATIONDIALOG_H

#include <QDialog>

class QLineEdit;

class AuthenticationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AuthenticationDialog(const QString &hostname, QWidget *parent = 0);

signals:

private slots:
    void accept();

private:
    QLineEdit *m_userEdit;
    QLineEdit *m_passwordEdit;

};

#endif // AUTHENTICATIONDIALOG_H
