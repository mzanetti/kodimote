# A name for the project
project(plasma-xbmcremote)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ../../../ # Hack: not in the mood to create a clean CMakeLists for the lib
   )
 
# We add our source code here
set(xbmcremote_engine_SRCS xbmcremoteengine.cpp

xbmcservice.cpp
xbmccontainer.cpp
hostcontainer.cpp
hostselector.cpp
hostsjob.cpp
xbmcjob.cpp
playerservice.cpp
playercontainer.cpp
playerjob.cpp

../../../libxbmcremote/xbmc.cpp
../../../libxbmcremote/xbmcdownload.cpp
../../../libxbmcremote/xbmcconnection.cpp
../../../libxbmcremote/xbmchostmodel.cpp
../../../libxbmcremote/xbmcdiscovery.cpp

../../../libxbmcremote/xdebug.cpp


../../../libxbmcremote/xbmcmodel.cpp
../../../libxbmcremote/xbmcfiltermodel.cpp

../../../libxbmcremote/xbmcmodelitem.cpp
../../../libxbmcremote/xbmclibrary.cpp
../../../libxbmcremote/libraryitem.cpp
../../../libxbmcremote/audiolibrary.cpp
../../../libxbmcremote/videolibrary.cpp
../../../libxbmcremote/albums.cpp
../../../libxbmcremote/songs.cpp
../../../libxbmcremote/tvshows.cpp
../../../libxbmcremote/seasons.cpp
../../../libxbmcremote/episodes.cpp
../../../libxbmcremote/musicvideos.cpp
../../../libxbmcremote/movies.cpp
../../../libxbmcremote/artists.cpp
../../../libxbmcremote/shares.cpp
../../../libxbmcremote/files.cpp
../../../libxbmcremote/recentitems.cpp
../../../libxbmcremote/channelgroups.cpp
../../../libxbmcremote/channels.cpp

../../../libxbmcremote/imagecache.cpp

../../../libxbmcremote/playlistitem.cpp
../../../libxbmcremote/audioplaylistitem.cpp

../../../libxbmcremote/player.cpp
../../../libxbmcremote/audioplayer.cpp
../../../libxbmcremote/videoplayer.cpp
../../../libxbmcremote/pictureplayer.cpp

../../../libxbmcremote/playlist.cpp
../../../libxbmcremote/videoplaylist.cpp
../../../libxbmcremote/videoplaylistitem.cpp
../../../libxbmcremote/audioplaylist.cpp
../../../libxbmcremote/pictureplaylist.cpp


../../../libxbmcremote/keys.cpp
../../../libxbmcremote/eventclient.cpp

)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_engine_xbmcremote ${xbmcremote_engine_SRCS})
target_link_libraries(plasma_engine_xbmcremote
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_PLASMA_LIBS}
                      ${QT_QTNETWORK_LIBRARY}
                      ${QT_QTDECLARATIVE_LIBRARY}
                      qjson)
 
install(TARGETS plasma_engine_xbmcremote
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
 install(FILES org.kde.xbmcremote.Hosts.operations org.kde.xbmcremote.Xbmc.operations org.kde.xbmcremote.Player.operations
        DESTINATION ${DATA_INSTALL_DIR}/plasma/services)
        
install(FILES plasma-engine-xbmcremote.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
