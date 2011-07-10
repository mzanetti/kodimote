# Add more folders to ship with the application, here
folder_01.source = qml/xbmcremote
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += network

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += systeminfo

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    settingsdialog.cpp \
    xbmc/xbmc.cpp \
    xbmc/xbmcconnection.cpp \
    xbmc/xbmcmodel.cpp \
    xbmc/player.cpp \
    xbmc/audioplayer.cpp \
    xbmc/videoplayer.cpp \
    xbmc/playlist.cpp \
    xbmc/audioplaylist.cpp \
    xbmc/videoplaylist.cpp \
    xbmc/playlistitem.cpp \
    xbmc/audioplaylistitem.cpp \
    xbmc/videoplaylistitem.cpp \
    xbmc/audiolibrary.cpp \
    xbmc/artists.cpp \
    xbmc/albums.cpp \
    xbmc/songs.cpp \
    xbmc/musicvideos.cpp \
    xbmc/tvshows.cpp \
    xbmc/seasons.cpp \
    xbmc/episodes.cpp \
    xbmc/videolibrary.cpp \
    xbmc/files.cpp \
    xbmc/shares.cpp \
    xbmc/movies.cpp \
    xbmc/keys.cpp

# We actually don't use the QMLApplicationViewer any more... It's pri file,
# however contains a lot of deplayment stuff so we still keep this here:

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()


HEADERS += settingsdialog.h \
    xbmc/xbmc.h \
    xbmc/xbmcconnection.h \
    xbmc/xbmcmodel.h \
    xbmc/player.h \
    xbmc/audioplayer.h \
    xbmc/videoplayer.h \
    xbmc/xbmcconnection_p.h \
    xbmc/playlist.h \
    xbmc/audioplaylist.h \
    xbmc/playlistitem.h \
    xbmc/audiolibrary.h \
    xbmc/artists.h \
    xbmc/albums.h \
    xbmc/songs.h \
    xbmc/files.h \
    xbmc/shares.h \
    xbmc/movies.h \
    xbmc/musicvideos.h \
    xbmc/tvshows.h \
    xbmc/seasons.h \
    xbmc/episodes.h \
    xbmc/videolibrary.h \
    xbmc/videoplaylist.h \
    xbmc/audioplaylistitem.h \
    xbmc/videoplaylistitem.h \
    xbmc/keys.h

LIBS += -lqjson

OTHER_FILES += \
    qtc_packaging/meego.spec \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    }
    INSTALLS += target
}

#contains (MEEGO_FLAVOR, "harmattan") { # This will be added soon, in the meantime use workaround
exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h") {
    message(Harmattan build)
    DEFINES += Q_WS_MAEMO_6
    target.path = /opt/usr/bin
    CONFIG += qmsystem2
    DEFINES += Q_WS_MAEMO_6
    SOURCES += meegohelper.cpp
    HEADERS += meegohelper.h
}

symbian:TARGET.UID3 = 0xE1297420

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices
