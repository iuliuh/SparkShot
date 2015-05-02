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
	src/drawingboard.cpp \
	src/controller.cpp \
	src/preferences.cpp \
	src/quicksettingsdialog.cpp \
	src/settingsdialog.cpp \
	src/toolbardialog.cpp \
    src/keysequenceedit.cpp

HEADERS  += \
	src/toolbar.h \
	src/defines.h \
	src/drawingboard.h \
	src/uploaddialog.h \
	src/colorpicker.h \
	src/controller.h \
	src/preferences.h \
	src/quicksettingsdialog.h \
	src/settingsdialog.h \
	src/toolbardialog.h \
    src/keysequenceedit.h

FORMS += \
	src/uploaddialog.ui \
	src/settingsdialog.ui

RESOURCES += \
	src/resources.qrc

INCLUDEPATH += src/
