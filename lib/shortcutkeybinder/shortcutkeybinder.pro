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

SOURCES += shortcutkeybinder.cpp

HEADERS += shortcutkeybinder.h\
	shortcutkeybinder_global.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}

unix:!macx {
	INCLUDEPATH += /usr/include/glib-2.0
	INCLUDEPATH += /usr/include/gtk-2.0
	INCLUDEPATH += /usr/include/atk-1.0
	INCLUDEPATH += /usr/include/cairo
	INCLUDEPATH += /usr/include/pango-1.0
	INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
	INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
	INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include

	LIBS += -L/usr/lib/x86_64-linux-gnu -lgtk-x11-2.0
	LIBS += -L/usr/lib/x86_64-linux-gnu -lkeybinder
}
