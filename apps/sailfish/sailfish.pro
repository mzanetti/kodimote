include(../../config.pri)
include(../../i18n/i18n.pri)

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/sailfishhelper.cpp

HEADERS += \
    src/sailfishhelper.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    xbmcremote.desktop \
    qml/main.qml \
    qml/pages/MainPage.qml \
    qml/pages/AddHostDialog.qml \
    qml/pages/ConnectPage.qml \
    qml/pages/BrowserPage.qml \
    qml/Thumbnail.qml
    qml/icons/icon-cover-stop.png

