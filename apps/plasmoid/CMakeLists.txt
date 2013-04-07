include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../libxbmcremote )

set(xbmcremote_SRCS
    xbmcremote.cpp
    declarativepopup.cpp

    )

kde4_add_plugin(plasma_applet_xbmcremote ${xbmcremote_SRCS})

link_directories(${CMAKE_BINARY_DIR}/libxbmcremote)

target_link_libraries(plasma_applet_xbmcremote ${KDE4_PLASMA_LIBS}
        ${QT_QTDECLARATIVE_LIBRARY} xbmcremote)

install(TARGETS plasma_applet_xbmcremote DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-xbmcremote.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(DIRECTORY qml/ DESTINATION ${DATA_INSTALL_DIR}/xbmcremote/qml)

#add_subdirectory(icons)

install(FILES
        xbmcremote.svgz
        DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/icons/)

#target_link_libraries(plasma_applet_xbmcremote ${KDE4_KCMUTILS_LIBS})
