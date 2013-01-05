TARGET = xbmcremote
TEMPLATE = app

QT += quick

INCLUDEPATH += $$top_srcdir
LIBS += -L$$top_builddir/libxbmcremote -lxbmcremote

SOURCES += main.cpp

message($$top_builddir)
qml.files = qml
qml.path = $$top_builddir/apps/ubuntu/

INSTALLS += qml

PRE_TARGETDEPS += ../../libxbmcremote/libxbmcremote.a
