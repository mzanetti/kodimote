TEMPLATE = app

TARGET = xbmcremote

QT += declarative network

INCLUDEPATH += $$top_srcdir

LIBS += -L$$top_builddir/libxbmcremote -lxbmcremote -lqjson

include(../../i18n/i18n.pri)

RESOURCES += xbmcremote.qrc

maemo5: {
    QT += maemo5

    CONFIG += mobility12
    MOBILITY += contacts
    LIBS += -lQtContacts

    # .qmake.cache.in doesn't seem to work for fremantle...
    INCLUDEPATH += $$PWD/../../
    LIBS += -L../../libxbmcremote/

    target.path = /opt/$${TARGET}/bin

    desktopfile.files = xbmcremote.desktop
    desktopfile.path = /usr/share/applications/

    qml.files = qml
    qml.path = /opt/$${TARGET}

    INSTALLS += desktopfile qml target
} else {

    target.path = $$top_builddir/apps/generic

    qml.files = qml
    qml.path = $$top_builddir/apps/generic/

    INSTALLS += target qml
}


SOURCES += main.cpp \
            ui/mainwindow.cpp \
            ui/settingsdialog.cpp \
            ui/connectdialog.cpp \
            ui/aboutdialog.cpp \
            ui/authenticationdialog.cpp \
            qmlapplicationviewer/qmlapplicationviewer.cpp \


HEADERS += ui/mainwindow.h \
            ui/settingsdialog.h \
            ui/connectdialog.h \
            ui/aboutdialog.h \
            ui/authenticationdialog.h \
            qmlapplicationviewer/qmlapplicationviewer.h \

PRE_TARGETDEPS += ../../libxbmcremote/libxbmcremote.a
