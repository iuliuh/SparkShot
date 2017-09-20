QT += gui-private
unix:!macx:{ QT += x11extras }

CONFIG += qxt

QXT += core gui widgets

DEFINES += \
    BUILD_QXT_CORE \
    BUILD_QXT_GUI

DEPENDPATH += \
    $$PWD/gui \
    $$PWD/core

INCLUDEPATH += \
    $$PWD/core \
    $$PWD/gui

HEADERS  += \
    $$PWD/core/qxtglobal.h \
    $$PWD/gui/qxtglobalshortcut_p.h \
    $$PWD/gui/qxtglobalshortcut.h

SOURCES  += \
    $$PWD/core/qxtglobal.cpp \
    $$PWD/gui/qxtglobalshortcut.cpp

unix:!macx {
        CONFIG += X11

        SOURCES += \
           $$PWD/gui/qxtglobalshortcut_x11.cpp

        HEADERS += \
            $$PWD/gui/x11info.h
}

macx: {
        SOURCES += \
            $$PWD/gui/qxtglobalshortcut_mac.cpp

        QMAKE_LFLAGS += -framework Carbon -framework CoreFoundation
}

win32: {
        SOURCES += \
            $$PWD/gui/qxtglobalshortcut_win.cpp
}
