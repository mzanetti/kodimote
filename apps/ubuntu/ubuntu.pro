include(../../config.pri)

QT += quick

SOURCES += main.cpp

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
message("fdsfsdfdsfdssad")
    qml.files = qml
    qml.path = $$OUT_PWD

    INSTALLS += qml
}
