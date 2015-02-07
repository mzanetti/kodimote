TARGET = kodimote
TEMPLATE = app

top_srcdir = $$PWD

INCLUDEPATH += $$top_srcdir
contains(QT_VERSION, ^5\\..\\..*) {
  LIBS += -L../../libkodimote -lkodimote
} else {
  LIBS += -L../../libkodimote -lkodimote -lqjson
}
#Needed by qmake to figure that this is a dep.
PRE_TARGETDEPS += ../../libkodimote/libkodimote.a

RESOURCES += ../../libkodimote/qml/qml.qrc
