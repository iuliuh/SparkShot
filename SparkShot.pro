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
