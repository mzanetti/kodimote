include(../../config.pri)
include(../../i18n/i18n.pri)

QT += quick

SOURCES += main.cpp \
            ubuntuhelper.cpp

HEADERS += ubuntuhelper.h

# debian/rules builds with CONFIG+=package
package {
    qml.files = qml
    qml.path = /usr/share/xbmcremote/

    target.path = /usr/bin/

    desktopfile.files = xbmcremote.desktop
    desktopfile.path = /usr/share/applications/

    icon.files = xbmcremote-square.svg
    icon.path = /usr/share/icons/hicolor/scalable/apps/


    INSTALLS += target qml desktopfile icon

} else {
    qml.files = qml
    qml.path = $$OUT_PWD

    icon.files = xbmcremote80.png
    icon.path = $$OUT_PWD

    INSTALLS += qml icon
}
