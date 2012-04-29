
QT += network

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# translations
translations.files = i18n/xbmcremote_de.qm \
    i18n/xbmcremote_nl.qm \
    i18n/xbmcremote_pt.qm \
    i18n/xbmcremote_el.qm \

translations.path = /opt/xbmcremote/i18n/
INSTALLS += translations

# Sources used on all platforms
SOURCES += main.cpp \
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
    xbmc/xbmchostmodel.cpp \
    ui/authenticationdialog.cpp \
    xbmc/xbmcdownload.cpp \


# header used on all platforms
HEADERS += xbmc/xbmc.h \
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
    xbmc/xbmchostmodel.h \
    ui/authenticationdialog.h \
    xbmc/xbmcdownload.h \

# to make lupdate parse QML files AND a pro file, the QML files need to be added here.
# edit and/or call ./messages.sh to extend/update the translations
lupdate {
    SOURCES += qml/xbmcremote/harmattan/main.qml \
        qml/xbmcremote/harmattan/AuthenticationSheet.qml \
        qml/xbmcremote/harmattan/ConnectionSheet.qml \
        qml/xbmcremote/harmattan/ItemDetails.qml \
        qml/xbmcremote/harmattan/MainPage.qml \
        qml/xbmcremote/harmattan/NoConnectionPage.qml \
        qml/xbmcremote/harmattan/NowPlayingPage.qml \
        qml/xbmcremote/harmattan/NowPlayingDetails.qml \
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
        qml/xbmcremote/fremantle/HomeMenu.qml \
}


# MeeGo 1.2 Harmattan specific stuff
contains(MEEGO_EDITION,harmattan) {
    message(Harmattan build)
    target.path = /opt/usr/bin
    QT += dbus
    CONFIG += qmsystem2 mobility meego
    MOBILITY = connectivity systeminfo contacts
    DEFINES += Q_WS_MAEMO_6

    # Speed up launching on MeeGo/Harmattan when using applauncherd daemon
    CONFIG += qdeclarative-boostable

    SOURCES += meegohelper.cpp \
            nfchandler.cpp

    HEADERS += meegohelper.h \
            nfchandler.h

    INCLUDEPATH += /usr/include/resource/qt4
    LIBS += -lresourceqt -lqjson
    PKGCONFIG += libtuiclient

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

    qmldir.source = qml/xbmcremote/harmattan
    qmldir.target = qml

    splash.files = splash.png
    splash.path = /opt/$${TARGET}
    INSTALLS += splash

}

unix:!meego:!maemo5:!symbian {
    SOURCES += ui/mainwindow.cpp \
        ui/settingsdialog.cpp \
        ui/connectdialog.cpp \
        ui/aboutdialog.cpp

    HEADERS += ui/mainwindow.h \
        ui/settingsdialog.h \
        ui/connectdialog.h \
        ui/aboutdialog.h

    LIBS += -lqjson

    qmldir.source = qml/xbmcremote/fremantle
    qmldir.target = qml
}

# Maemo 5 specific stuff
maemo5 {
    QT += dbus
    CONFIG += mobility12
    MOBILITY += contacts
    LIBS += -lQtContacts

    LIBS += -lqjson

    SOURCES += ui/settingsdialog.cpp \
        ui/connectdialog.cpp \
        ui/aboutdialog.cpp \
        ui/mainwindow.cpp

    HEADERS += ui/settingsdialog.h \
        ui/connectdialog.h \
        ui/aboutdialog.h \
        ui/mainwindow.h

    qmldir.source = qml/xbmcremote/fremantle
    qmldir.target = qml
}

# Currently disabled on symbian as it requires either Unix or Qt 4.8
!symbian {
    HEADERS += xbmc/xbmcdiscovery.h \

    SOURCES += xbmc/xbmcdiscovery.cpp \
}


# Symbian specific stuff
# Currently tested on Symbian Anna and Belle - You need to create a smart installer for Anna which pulls in Qt. Belle comes with Qt preinstalled)
symbian {
    message("Symbian build")
    TARGET.UID3 = 0xE1297420

    SOURCES += symbianhelper.cpp \
            nfchandler.cpp

    HEADERS += symbianhelper.h \
            nfchandler.h

    CONFIG += mobility
    MOBILITY += connectivity

    # Allow network access on Symbian
    TARGET.CAPABILITY += NetworkServices ReadDeviceData WriteDeviceData LocalServices

    # You need to compile libqjson and deploy it to Symbian sysroot on your own for now
    # Don't forget to edit the capabilities there to the same as here.
    # This will add the lib from the sysroot to the package.
    LIBS += -lqjson.lib
#    addFiles.sources = xbmc_qjson.dll
#    addFiles.path = /sys/bin
#    DEPLOYMENT += addFiles

    qmldir.source = qml/xbmcremote/symbian
    qmldir.target = qml

}

RESOURCES += \
    xbmcremote.qrc


DEPLOYMENTFOLDERS = qmldir
# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
