project(xbmcremote)

find_package(Qt4 REQUIRED)
find_package(KDE4 "4.10.0" REQUIRED)

include (KDE4Defaults)
include(MacroOptionalAddSubdirectory)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDES} ${CMAKE_INCLUDE_PATH})
link_directories(${CMAKE_LIBRARY_PATH})

add_subdirectory(libxbmcremote)
add_subdirectory(apps)

macro_display_feature_log()

