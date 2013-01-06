include(../../config.pri)
include(../../i18n/i18n.pri)

TARGET = xbmcremote

QT += dbus declarative
MOBILITY = connectivity systeminfo contacts feedback
CONFIG += qmsystem2 mobility meego qdeclarative-boostable
PKGCONFIG += libtuiclient

INCLUDEPATH += $$top_srcdir /usr/include/resource/qt4

LIBS += -L$$OUT_PWD/../../libxbmcremote -lxbmcremote -lresourceqt -lqjson

DEFINES += Q_WS_MAEMO_6


SOURCES += main.cpp \
            gesturehelper.cpp \
            meegohelper.cpp \
            nfchandler.cpp \
            rumbleeffect.cpp

HEADERS += meegohelper.h \
            nfchandler.h \
            rumbleeffect.h \
            gesturehelper.h

target.path = /opt/$${TARGET}/bin

qmlfiles.files = qml/
qmlfiles.path = /opt/$${TARGET}

splash.files = splash.png
splash.path = /opt/$${TARGET}

desktopfile.files = xbmcremote.desktop
desktopfile.path = /usr/share/applications/

icon.files = xbmcremote80.png
icon.path = /usr/share/icons/hicolor/80x80/apps/

INSTALLS += target qmlfiles splash desktopfile icon

PRE_TARGETDEPS += ../../libxbmcremote/libxbmcremote.a
