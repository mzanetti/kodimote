include(../../config.pri)
include(../../i18n/i18n.pri)

QT += declarative network

RESOURCES += kodimote.qrc

maemo5: {
    QT += maemo5

    CONFIG += mobility12
    MOBILITY += contacts
    LIBS += -lQtContacts

    target.path = /opt/$${TARGET}/bin

    desktopfile.files = kodimote.desktop
    desktopfile.path = /usr/share/applications/hildon/

    icon.files = kodimote64.png
    icon.path = /opt/usr/share/icons/hicolor/64x64/apps/

    qml.files = qml
    qml.path = /opt/$${TARGET}

    INSTALLS += desktopfile qml target icon
} else {

#    target.path = $$top_builddir/apps/generic

    qml.files = qml
    qml.path = $$OUT_PWD

#    INSTALLS += target qml
    INSTALLS += qml
}


SOURCES += main.cpp \
            ui/mainwindow.cpp \
            ui/settingsdialog.cpp \
            ui/aboutdialog.cpp \
            ui/authenticationdialog.cpp \
            qmlapplicationviewer/qmlapplicationviewer.cpp \
    ui/connectiondialog.cpp


HEADERS += ui/mainwindow.h \
            ui/settingsdialog.h \
            ui/aboutdialog.h \
            ui/authenticationdialog.h \
            qmlapplicationviewer/qmlapplicationviewer.h \
    ui/connectiondialog.h
