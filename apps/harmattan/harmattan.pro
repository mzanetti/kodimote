include(../../config.pri)
include(../../i18n/i18n.pri)

QT += dbus declarative
MOBILITY = connectivity systeminfo contacts feedback
CONFIG += qmsystem2 mobility meego qdeclarative-boostable
PKGCONFIG += libtuiclient

contains(MEEGO_EDITION,harmattan) {
    DEFINES += Q_WS_MAEMO_6

    INCLUDEPATH += /usr/include/resource/qt4
    LIBS += -lresourceqt

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

    desktopfile.files = kodimote.desktop
    desktopfile.path = /usr/share/applications/

    icon.files = kodimote80.png
    icon.path = /usr/share/icons/hicolor/80x80/apps/

    INSTALLS += target qmlfiles splash desktopfile icon

} else:simulator {

    SOURCES += main.cpp \
                gesturehelper.cpp \
                nfchandler.cpp \
                rumbleeffect.cpp

    HEADERS +=  nfchandler.h \
                rumbleeffect.h \
                gesturehelper.h

    qml.files = qml
    qml.path = $$OUT_PWD

    INSTALLS += qml
}
