#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T01:42:16
#
#-------------------------------------------------

QT += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += no_keywords

TARGET = shortcutkeybinder
TEMPLATE = lib

DEFINES += SHORTCUTKEYBINDER_LIBRARY

SOURCES += \
	hotkeyworker.cpp \
    hotkeybinder.cpp

HEADERS += \
	shortcutkeybinder_global.h \
	hotkeyworker.h \
    hotkeybinder.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}

unix:!macx {
	CONFIG	+= link_pkgconfig
	PKGCONFIG += x11
}
