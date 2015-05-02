#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T01:42:16
#
#-------------------------------------------------

QT -= gui

TARGET = shortcutkeybinder
TEMPLATE = lib

DEFINES += SHORTCUTKEYBINDER_LIBRARY

SOURCES += shortcutkeybinder.cpp

HEADERS += shortcutkeybinder.h\
	shortcutkeybinder_global.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}
