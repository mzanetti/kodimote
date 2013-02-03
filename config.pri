TARGET = xbmcremote
TEMPLATE = app

top_srcdir = $$PWD

INCLUDEPATH += $$top_srcdir
contains(QT_VERSION, ^5\\..\\..*) {
  LIBS += -L../../libxbmcremote -lxbmcremote
} else {
  LIBS += -L../../libxbmcremote -lxbmcremote -lqjson
}
#Needed by qmake to figure that this is a dep.
PRE_TARGETDEPS += ../../libxbmcremote/libxbmcremote.a
