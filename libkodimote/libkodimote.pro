QT += concurrent network dbus
contains(QT_VERSION, ^5\\..\\..*) {
    DEFINES += QT5_BUILD
    QT += quick qml
} else {
    QT += declarative
}

packagesExist(sailfishapp) {
    DEFINES += SAILFISH
}

ubuntu {
    DEFINES += UBUNTU
}

TARGET = kodimote
TEMPLATE = lib
CONFIG += staticlib

DEFINES += LIBKODIMOTE_LIBRARY

LIBS += -lqjson

SOURCES +=  kodi.cpp \
            kodiconnection.cpp \
            kodimodel.cpp \
            player.cpp \
            audioplayer.cpp \
            videoplayer.cpp \
            playlist.cpp \
            audioplaylist.cpp \
            videoplaylist.cpp \
            playlistitem.cpp \
            audioplaylistitem.cpp \
            videoplaylistitem.cpp \
            audiolibrary.cpp \
            artists.cpp \
            albums.cpp \
            songs.cpp \
	    genres.cpp \
            musicvideos.cpp \
            tvshows.cpp \
            seasons.cpp \
            episodes.cpp \
            videolibrary.cpp \
            files.cpp \
            shares.cpp \
            movies.cpp \
            keys.cpp \
            pictures.cpp \
            pictureplayer.cpp \
            kodebug.cpp \
            kodilibrary.cpp \
            pictureplaylist.cpp \
            libraryitem.cpp \
            kodimodelitem.cpp \
            episodeitem.cpp \
            kodihostmodel.cpp \
            kodidownload.cpp \
            kodifiltermodel.cpp \
            imagecache.cpp \
            kodidiscovery.cpp \
            networkauthhandler.cpp \
            networkaccessmanagerfactory.cpp \
            settings.cpp \
            channels.cpp \
            channelgroups.cpp \
	    eventclient.cpp \
	    recentitems.cpp \
	    channelitem.cpp \
	    channelbroadcasts.cpp \
            recordings.cpp \
            pvrmenu.cpp \
            kodihost.cpp \
            addonsource.cpp \
            profiles.cpp \
            profileitem.cpp \
            mpris2/mpriscontroller.cpp \
            mpris2/mprisplayer.cpp \
            mpris2/mprisapplication.cpp \
            protocolhandlers/protocolhandler.cpp \
            protocolhandlers/youtubeprotocolhandler.cpp \
            protocolhandlers/protocolmanager.cpp \
            protocolhandlers/nativeprotocolhandler.cpp \
            platformhelper.cpp

HEADERS += libkodimote_global.h \
           kodi.h \
           kodiconnection.h \
           kodimodel.h \
           player.h \
           audioplayer.h \
           videoplayer.h \
           kodiconnection_p.h \
           playlist.h \
           audioplaylist.h \
           playlistitem.h \
           audiolibrary.h \
           artists.h \
           albums.h \
           songs.h \
	   genres.h \
           files.h \
           shares.h \
           movies.h \
           musicvideos.h \
           tvshows.h \
           seasons.h \
           episodes.h \
           videolibrary.h \
           videoplaylist.h \
           audioplaylistitem.h \
           videoplaylistitem.h \
           keys.h \
           pictures.h \
           pictureplayer.h \
           kodebug.h \
           kodilibrary.h \
           pictureplaylist.h \
           libraryitem.h \
           kodimodelitem.h \
           episodeitem.h \
           kodihostmodel.h \
           kodidownload.h \
           kodifiltermodel.h \
           imagecache.h \
           kodidiscovery.h \
           networkauthhandler.h \
           networkaccessmanagerfactory.h \
           settings.h \
           channels.h \
           channelgroups.h \
	   eventclient.h \
	   recentitems.h \
	   channelitem.h \
	   channelbroadcasts.h \
           recordings.h \
	   pvrmenu.h \
           kodihost.h \
           addonsource.h \
           profiles.h \
           profileitem.h \
           mpris2/mpriscontroller.h \
           mpris2/mprisplayer.h \
           mpris2/mprisapplication.h \
           protocolhandlers/protocolhandler.h \
           protocolhandlers/youtubeprotocolhandler.h \
           protocolhandlers/protocolmanager.h \
           protocolhandlers/nativeprotocolhandler.h \
           platformhelper.h
