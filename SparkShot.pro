#-------------------------------------------------
#
# Project created by QtCreator 2015-04-12T23:16:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SparkShot
TEMPLATE = app
CONFIG += no_keywords

SOURCES += \
	src/main.cpp \
	src/toolbar.cpp \
	src/uploaddialog.cpp \
	src/colorpicker.cpp \
	src/dialog.cpp \
	src/drawingboard.cpp \
	src/controller.cpp \
	src/settingsdialog.cpp \
	src/preferences.cpp \
    src/gnulinuxtray.cpp

HEADERS  += \
	src/toolbar.h \
	src/defines.h \
	src/dialog.h \
	src/drawingboard.h \
	src/uploaddialog.h \
	src/colorpicker.h \
	src/controller.h \
	src/settingsdialog.h \
	src/preferences.h \
    src/gnulinuxtray.h

FORMS += \
	src/uploaddialog.ui

RESOURCES += \
	src/resources.qrc

unix:!macx{
	INCLUDEPATH += /usr/include/libappindicator-0.1
	INCLUDEPATH += /usr/include/gtk-2.0
	INCLUDEPATH += /usr/include/glib-2.0
	INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
	INCLUDEPATH += /usr/include/cairo
	INCLUDEPATH += /usr/include/pango-1.0
	INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
	INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
	INCLUDEPATH += /usr/include/atk-1.0

	LIBS += -L/usr/lib/x86_64-linux-gnu -lgobject-2.0
	LIBS += -L/usr/lib/x86_64-linux-gnu -lappindicator
	LIBS += -L/usr/lib/x86_64-linux-gnu -lgtk-x11-2.0
	LIBS += -L/usr/lib/x86_64-linux-gnu -lkeybinder

	CONFIG	+= link_pkgconfig
	PKGCONFIG += gtkmm-3.0
}
