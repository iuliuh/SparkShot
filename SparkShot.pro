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
    main.cpp \
    screenshotarea.cpp \
    toolbar.cpp \
    uploaddialog.cpp \
    colorpicker.cpp \
    colorpickerdialog.cpp \
    dialog.cpp \
    settings.cpp

HEADERS  += \
    screenshotarea.h \
    toolbar.h \
    defines.h \
    uploaddialog.h \
    colorpicker.h \
    colorpickerdialog.h \
    dialog.h \
    settings.h

FORMS += \
    uploaddialog.ui

RESOURCES += \
    resources.qrc
