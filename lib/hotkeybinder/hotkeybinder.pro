#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T01:42:16
#
#-------------------------------------------------

QT += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += no_keywords

TARGET = hotkeybinder
TEMPLATE = lib

DEFINES += HOTKEYBINDER_LIBRARY

SOURCES += \
	hotkeybinder.cpp \
    hotkey.cpp

HEADERS += \
	hotkeybinder.h \
	hotkeybinder_global.h \
	defines.h \
	hotkey.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}

win32 {
	SOURCES += \
		winhotkeybinder.cpp

	HEADERS += \
		winhotkeybinder.h
}

unix:!macx {
	SOURCES += \
		gnulinuxhotkeybinder.cpp \
		gnulinuxhotkeyworker.cpp

	HEADERS += \
		gnulinuxhotkeyworker.h \
		gnulinuxhotkeybinder.h

	CONFIG += link_pkgconfig
	PKGCONFIG += x11
}
