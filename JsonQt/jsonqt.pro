#-------------------------------------------------
#
# Project created by QtCreator 2011-04-09T18:20:49
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = jsonqt
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = lib


SOURCES += lib/JsonRpcAdaptor.cpp \
lib/JsonRpcAdaptorPrivate.cpp \
lib/JsonRpc.cpp \
lib/JsonToProperties.cpp \
lib/JsonToVariant.cpp \
lib/ParseException.cpp \
lib/VariantToJson.cpp

HEADERS += lib/JsonRpcAdaptor.h \
lib/JsonRpcAdaptorPrivate.h \
lib/JsonRpc.h \
lib/JsonToProperties.h \
lib/JsonToVariant.h \
lib/ParseException.h \
lib/VariantToJson.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
