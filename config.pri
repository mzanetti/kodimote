TARGET = xbmcremote
TEMPLATE = app

top_srcdir = $$PWD

INCLUDEPATH += $$top_srcdir
LIBS += -L../../libxbmcremote -lxbmcremote -lqjson
#Needed by qmake to figure that this is a dep.
PRE_TARGETDEPS += ../../libxbmcremote/libxbmcremote.a
