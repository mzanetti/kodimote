contains(QT_VERSION, ^5\\..\\..*) {
    DEFINES += QT5_BUILD
    QT += network quick qml
} else {
    QT += network declarative
}

packagesExist(sailfishapp) {
    DEFINES += SAILFISH
}

TARGET = xbmcremote
TEMPLATE = lib
CONFIG += static

DEFINES += LIBXBMCREMOTE_LIBRARY

LIBS += -lqjson

SOURCES +=  xbmc.cpp \
            xbmcconnection.cpp \
            xbmcmodel.cpp \
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
            xdebug.cpp \
            xbmclibrary.cpp \
            pictureplaylist.cpp \
            libraryitem.cpp \
            xbmcmodelitem.cpp \
            episodeitem.cpp \
            xbmchostmodel.cpp \
            xbmcdownload.cpp \
            xbmcfiltermodel.cpp \
            imagecache.cpp \
            xbmcdiscovery.cpp \
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
	    xbmchost.cpp

HEADERS += libxbmcremote_global.h \
           xbmc.h \
           xbmcconnection.h \
           xbmcmodel.h \
           player.h \
           audioplayer.h \
           videoplayer.h \
           xbmcconnection_p.h \
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
           xdebug.h \
           xbmclibrary.h \
           pictureplaylist.h \
           libraryitem.h \
           xbmcmodelitem.h \
           episodeitem.h \
           xbmchostmodel.h \
           xbmcdownload.h \
           xbmcfiltermodel.h \
           imagecache.h \
           xbmcdiscovery.h \
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
	   xbmchost.h
