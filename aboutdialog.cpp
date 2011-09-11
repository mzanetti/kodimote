#include "aboutdialog.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QDesktopServices>
#include <QUrl>

#define VERSION_STRING "0.3.1"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowTitle(tr("About") + " Xbmcremote");

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setSpacing(20);

#if Q_WS_MAEMO_5
    QPixmap icon = QPixmap(":/logo_l").scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
#else
    QPixmap icon = QPixmap(":/logo_l").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
#endif
    QLabel *iconLabel = new QLabel();
    iconLabel->setAlignment(Qt::AlignTop);
    iconLabel->setPixmap(icon);
    hlayout->addWidget(iconLabel);

    QVBoxLayout *vlayout = new QVBoxLayout;
    hlayout->addLayout(vlayout);

    QLabel *label = new QLabel(QString("Xbmcremote ") + VERSION_STRING);
    QFont font;
    font.setPixelSize(24);
    label->setFont(font);
    vlayout->addWidget(label);

    QLabel *label1 = new QLabel("Copyright\nMichael Zanetti <michael_zanetti@gmx.net>");
    vlayout->addWidget(label1);

    QLabel *label2 = new QLabel("Thanks to\n" \
                                "Xbmc Development Team (artwork)\n" \
                                "Johannes Siipola (artwork)");
    vlayout->addWidget(label2);


    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *closeButton = new QPushButton(tr("Close"));
    buttonLayout->addWidget(closeButton);
    connect(closeButton, SIGNAL(clicked()), SLOT(close()));

    QPushButton *donateButton = new QPushButton(tr("Donate"));
    buttonLayout->addWidget(donateButton);
    connect(donateButton, SIGNAL(clicked()), SLOT(openDonationPage()));

    vlayout->addLayout(buttonLayout);

    setLayout(hlayout);
}

void AboutDialog::openDonationPage()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CWFYRZH8XNYF2"));
}
