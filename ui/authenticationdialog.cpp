#include "authenticationdialog.h"
#include "xbmc/xbmc.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>

AuthenticationDialog::AuthenticationDialog(const QString &hostname, QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowTitle("XbmcRemote - " + tr("Authentication"));

#ifdef Q_WS_MAEMO_5
    QHBoxLayout *layout = new QHBoxLayout();

#else
    QVBoxLayout *layout = new QVBoxLayout();

#endif
    setLayout(layout);

    QVBoxLayout *vLayout = new QVBoxLayout();
    QLabel *mainLabel = new QLabel(tr("Host %1 requires authentication").arg(hostname));
    vLayout->addWidget(mainLabel);

    QGridLayout *gridLayout = new QGridLayout();
    vLayout->addLayout(gridLayout);

    QLabel *userLabel = new QLabel(tr("Username"));
    gridLayout->addWidget(userLabel, 0, 0);

    m_userEdit = new QLineEdit();
    gridLayout->addWidget(m_userEdit, 0, 1);

    QLabel *passwordLabel = new QLabel(tr("Password"));
    gridLayout->addWidget(passwordLabel, 1, 0);

    m_passwordEdit = new QLineEdit();
    gridLayout->addWidget(m_passwordEdit, 1, 1);

    layout->addLayout(vLayout);

#ifdef Q_WS_MAEMO_5
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
#else
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
#endif
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    layout->addWidget(buttonBox);
}

void AuthenticationDialog::accept()
{
    Xbmc::instance()->setAuthCredentials(m_userEdit->text(), m_passwordEdit->text());
    QDialog::accept();
}
