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
LIBS += -lQtSystemInfo

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

SOURCES += main.cpp \
    ui/settingsdialog.cpp \
    ui/connectdialog.cpp \
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
    xbmc/keys.cpp \
    settings.cpp \
    networkaccessmanagerfactory.cpp \
    xbmc/pictures.cpp \
    xbmc/pictureplayer.cpp \
    xbmc/xdebug.cpp \
    xbmc/xbmclibrary.cpp \
    xbmc/pictureplaylist.cpp \
    xbmc/libraryitem.cpp \
    networkauthhandler.cpp \
    xbmc/xbmcmodelitem.cpp \
    xbmc/episodeitem.cpp \
    xbmc/xbmcdiscovery.cpp \
    xbmc/xbmchostmodel.cpp \
    ui/authenticationdialog.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

translations.files = i18n/xbmcremote_de.qm
translations.path = /opt/xbmcremote/i18n/
INSTALLS += translations

HEADERS += ui/settingsdialog.h \
    ui/connectdialog.h \
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
    xbmc/keys.h \
    settings.h \
    networkaccessmanagerfactory.h \
    xbmc/pictures.h \
    xbmc/pictureplayer.h \
    xbmc/xdebug.h \
    xbmc/xbmclibrary.h \
    xbmc/pictureplaylist.h \
    xbmc/libraryitem.h \
    networkauthhandler.h \
    xbmc/xbmcmodelitem.h \
    xbmc/episodeitem.h \
    xbmc/xbmcdiscovery.h \
    xbmc/xbmchostmodel.h \
    ui/authenticationdialog.h

LIBS += -lqjson

OTHER_FILES += \
    qtc_packaging/meego.spec \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

lupdate {
    SOURCES += qml/xbmcremote/harmattan/main.qml \
        qml/xbmcremote/harmattan/AuthenticationSheet.qml \
        qml/xbmcremote/harmattan/ConnectionSheet.qml \
        qml/xbmcremote/harmattan/ItemDetails.qml \
        qml/xbmcremote/harmattan/MainPage.qml \
        qml/xbmcremote/harmattan/NoConnectionPage.qml \
        qml/xbmcremote/harmattan/NowPlayingPage.qml \
        qml/xbmcremote/harmattan/BrowserPage.qml \
        qml/xbmcremote/harmattan/Keypad.qml \
        qml/xbmcremote/harmattan/PictureControlsPage.qml \
        qml/xbmcremote/harmattan/PlaylistPage.qml \
        qml/xbmcremote/harmattan/SettingsSheet.qml \
        qml/xbmcremote/harmattan/WriteNfcTagSheet.qml \
        qml/xbmcremote/fremantle/AudioPlaylist.qml \
        qml/xbmcremote/fremantle/ItemDetails.qml \
        qml/xbmcremote/fremantle/LibraryView.qml \
        qml/xbmcremote/fremantle/NowPlayingView.qml \
}

contains(MEEGO_EDITION,harmattan) {
    message(Harmattan build)
    target.path = /opt/usr/bin
    QT += dbus
    CONFIG += qmsystem2 mobility
    MOBILITY = connectivity
    DEFINES += Q_WS_MAEMO_6
    SOURCES += meegohelper.cpp \
            nfchandler.cpp

    HEADERS += meegohelper.h \
            nfchandler.h

    INCLUDEPATH += /usr/include/resource/qt4
    LIBS += -lresourceqt
} else {
    SOURCES += ui/aboutdialog.cpp \
               ui/mainwindow.cpp

    HEADERS += ui/aboutdialog.h \
               ui/mainwindow.h
}

maemo5 {
    QT += dbus
}

symbian:TARGET.UID3 = 0xE1297420

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices


symbian {
myembeddedsis.pkg_postrules = "@/"/home/micha/Develop/qjson-build-remote/qjson_qt-4_7_4_symbianBelle.sis",(0xEF76E062)"\
DEPLOYMENT += myembeddedsis
}

RESOURCES += \
    xbmcremote.qrc

splash.files = splash.png
splash.path = /opt/$${TARGET}
INSTALLS += splash
