include(../../config.pri)
include(../../i18n/i18n.pri)

QT += quick

SOURCES += main.cpp \
            ubuntuhelper.cpp

HEADERS += ubuntuhelper.h

target.path = /
qml.files = qml
qml.path = /
icon.files = kodimote.svg
icon.path = /
desktopfile.files = kodimote.desktop
desktopfile.path = /
apparmor.files = kodimote.json
apparmor.path = /

INSTALLS += qml icon desktopfile apparmor target


#This creates the manifest.json file, it is the description file for the
#click package.

#figure out the current build architecture
CLICK_ARCH=$$system(dpkg-architecture -qDEB_HOST_ARCH)


# substitute the architecture in the manifest file
manifest_file.output   = manifest.json
manifest_file.CONFIG  += no_link \
                         add_inputs_as_makefile_deps\
                         target_predeps
manifest_file.commands = sed s/@CLICK_ARCH@/$$CLICK_ARCH/g ${QMAKE_FILE_NAME} > ${QMAKE_FILE_OUT}
manifest_file.input = UBUNTU_MANIFEST_FILE
QMAKE_EXTRA_COMPILERS += manifest_file

#do not remove this line, it is required by the IDE even if you do
#not substitute variables in the manifest file
UBUNTU_MANIFEST_FILE = $$PWD/manifest.json.in

#installation path of the manifest file
mfile.path = /
mfile.CONFIG += no_check_exist
mfile.files  += $$OUT_PWD/manifest.json
INSTALLS+=mfile
