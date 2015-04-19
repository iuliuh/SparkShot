#-------------------------------------------------
#
# Project created by QtCreator 2015-04-12T23:16:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SparkShot
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/screenshotarea.cpp \
    src/toolbar.cpp \
    src/uploaddialog.cpp \
    src/colorpicker.cpp \
    src/colorpickerdialog.cpp \
    src/dialog.cpp \
    src/settings.cpp

HEADERS  += \
    src/screenshotarea.h \
    src/toolbar.h \
    src/defines.h \
    src/uploaddialog.h \
    src/colorpicker.h \
    src/colorpickerdialog.h \
    src/dialog.h \
    src/settings.h

FORMS += \
    src/uploaddialog.ui

RESOURCES += \
    src/resources.qrc
