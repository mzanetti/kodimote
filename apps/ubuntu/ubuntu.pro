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

    icon.files = xbmcremote80.png
    icon.path = /usr/share/pixmaps/

    INSTALLS += target qml desktopfile icon

} else {
    qml.files = qml
    qml.path = apps/ubuntu/

    INSTALLS += target qml
}
