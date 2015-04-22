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
    src/toolbar.cpp \
    src/uploaddialog.cpp \
    src/colorpicker.cpp \
    src/dialog.cpp \
    src/settings.cpp \
    src/drawingboard.cpp \
    src/controller.cpp

HEADERS  += \
    src/toolbar.h \
    src/defines.h \
    src/dialog.h \
    src/settings.h \
    src/drawingboard.h \
    src/uploaddialog.h \
    src/colorpicker.h \
    src/controller.h

FORMS += \
    src/uploaddialog.ui

RESOURCES += \
    src/resources.qrc
