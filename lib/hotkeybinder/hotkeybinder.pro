#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T01:42:16
#
#-------------------------------------------------

include(qxtglobalshortcut5/qxt.pri)

QT += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += no_keywords

TARGET = hotkeybinder
TEMPLATE = lib

DEFINES += HOTKEYBINDER_LIBRARY

SOURCES += \
    hotkeybinder.cpp \

HEADERS += \
    hotkeybinder_global.h \
    hotkeybinder.h

win32 {
	target.path = $$PWD/../../deploy/win/packages/com.devtelsoftware.sparkshot/data
	INSTALLS += target
}

unix {
	target.path = /usr/lib
	INSTALLS += target
}

# Deploy
win32 {
	CONFIG(release, debug|release) {
		QMAKE_POST_LINK = cp $$OUT_PWD/"release/"$$TARGET".dll" $$PWD/../../deploy/win/packages/com.devtelsoftware.sparkshot/data
	}
}
