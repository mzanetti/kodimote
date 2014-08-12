
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

set(libxbmcremote_SRCS
    xbmc.cpp
    xbmcdownload.cpp
    xbmcconnection.cpp
    xbmchostmodel.cpp
    xbmchost.cpp
    xbmcdiscovery.cpp

    xdebug.cpp
    settings.cpp

    xbmcmodel.cpp
    xbmcfiltermodel.cpp

    xbmcmodelitem.cpp
    xbmclibrary.cpp
    libraryitem.cpp
    audiolibrary.cpp
    videolibrary.cpp
    albums.cpp
    songs.cpp
    tvshows.cpp
    seasons.cpp
    episodes.cpp
    musicvideos.cpp
    movies.cpp
    artists.cpp
    shares.cpp
    files.cpp
    recentitems.cpp
    channelgroups.cpp
    channels.cpp
    channelitem.cpp
    channelbroadcasts.cpp

    imagecache.cpp

    playlistitem.cpp
    audioplaylistitem.cpp

    player.cpp
    audioplayer.cpp
    videoplayer.cpp
    pictureplayer.cpp

    playlist.cpp
    videoplaylist.cpp
    videoplaylistitem.cpp
    audioplaylist.cpp
    pictureplaylist.cpp


    keys.cpp
    eventclient.cpp
)


kde4_add_library(xbmcremote STATIC ${libxbmcremote_SRCS})

target_link_libraries(xbmcremote ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY} ${QT_QTNETWORK_LIBRARY} qjson)

set_target_properties(xbmcremote PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION}
    COMPILE_FLAGS -fPIC
    )

install(TARGETS xbmcremote ${INSTALL_TARGETS_DEFAULT_ARGS})



