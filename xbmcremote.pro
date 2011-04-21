# Add more folders to ship with the application, here
folder_01.source = qml/xbmcremote
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += network

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
symbian:TARGET.UID3 = 0xE1297420

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    xbmc/xbmc.cpp \
    xbmc/player.cpp \
    xbmc/audioplayer.cpp \
    xbmc/videoplayer.cpp \
    xbmc/xbmcconnection.cpp \
    xbmc/playlist.cpp \
    xbmc/audioplaylist.cpp \
    xbmc/playlistitem.cpp \
    xbmc/audiolibrary.cpp \
    xbmc/artistitem.cpp \
    xbmc/albumitem.cpp \
    xbmc/songitem.cpp \
    xbmc/libraryitem.cpp \
    xbmc/files.cpp

# We actually don't use the QMLApplicationViewer any more... It's pri file,
# however contains a lot of deplayment stuff so we still keep this here:

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()


HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    xbmc/xbmc.h \
    xbmc/player.h \
    xbmc/audioplayer.h \
    xbmc/videoplayer.h \
    xbmc/xbmcconnection.h \
    xbmc/xbmcconnection_p.h \
    xbmc/playlist.h \
    xbmc/audioplaylist.h \
    xbmc/playlistitem.h \
    xbmc/audiolibrary.h \
    xbmc/artistitem.h \
    xbmc/albumitem.h \
    xbmc/songitem.h \
    xbmc/libraryitem.h \
    xbmc/files.h

maemo5: {
# change this to where you have QJsons headers
# Most desktops will install them to /usr/include/qjson/ but that
# doesn't get picked up for maemo builds
# if you check it out from git you have the headers in a dir called src/
INCLUDEPATH += ../qjson/src/
# change this to where you have libqjson compiled for maemo
# most like you want to install libqjson on the device and copy it over
LIBS += -L.
}
LIBS += -lqjson
