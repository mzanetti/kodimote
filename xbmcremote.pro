# Add more folders to ship with the application, here
folder_01.source = qml/xbmcremote
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += network

INCLUDEPATH += ../JsonQt/lib/

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
        LIBS += -L../JsonQt-build-maemo/ -ljsonqt
    } else {
        target.path = /usr/local/lib
        LIBS += -L../JsonQt-build-desktop/ -ljsonqt
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
    xbmc.cpp \
    player.cpp \
    audioplayer.cpp \
    videoplayer.cpp \
    xbmcconnection.cpp \
    playlist.cpp \
    audioplaylist.cpp \
    playlistitem.cpp \
    audiolibrary.cpp \
    artistitem.cpp \
    albumitem.cpp \
    songitem.cpp \
    libraryitem.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    xbmc.h \
    player.h \
    audioplayer.h \
    videoplayer.h \
    xbmcconnection.h \
    xbmcconnection_p.h \
    playlist.h \
    audioplaylist.h \
    playlistitem.h \
    audiolibrary.h \
    artistitem.h \
    albumitem.h \
    songitem.h \
    libraryitem.h
