include(../../config.pri)
include(../../i18n/i18n.pri)

QT += quick dbus

SOURCES += main.cpp \
            ubuntuhelper.cpp

HEADERS += ubuntuhelper.h

# Copy the png for the notification to the build dir for running on the desktop
QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$PWD/kodimote80.png) $$quote($$OUT_PWD)

# Install files into the click package
target.path = /
icon.files = kodimote.svg kodimote80.png
icon.path = /
desktopfile.files = kodimote.desktop
desktopfile.path = /
apparmor.files = kodimote.json
apparmor.path = /

INSTALLS += icon desktopfile apparmor target


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

RESOURCES += \
    resources.qrc \
    images.qrc
