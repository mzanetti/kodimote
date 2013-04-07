project(xbmcremote)

set (MINIMUM_NM_VERSION_REQUIRED "0.9.0")

# We need some features from QML components from kde-runtime >= 4.9.0, like
# TextField.qml's accepted() signal.
find_package(KDE4 "4.9.0" REQUIRED)
find_package(KDE4 REQUIRED)

include (KDE4Defaults)
include(MacroOptionalAddSubdirectory)

#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES} ${CMAKE_INCLUDE_PATH})
link_directories(${CMAKE_LIBRARY_PATH})

#include(ConfigureChecks.cmake)

add_subdirectory(libxbmcremote)
add_subdirectory(apps)

macro_display_feature_log()

