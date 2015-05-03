#-------------------------------------------------
#
# Project created by QtCreator 2015-04-12T23:16:40
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SparkShot
TEMPLATE = app
CONFIG += no_keywords

SOURCES += \
	main.cpp \
	toolbar.cpp \
	uploaddialog.cpp \
	colorpicker.cpp \
	drawingboard.cpp \
	controller.cpp \
	preferences.cpp \
	quicksettingsdialog.cpp \
	settingsdialog.cpp \
	toolbardialog.cpp \
	keysequenceedit.cpp

HEADERS  += \
	toolbar.h \
	defines.h \
	drawingboard.h \
	uploaddialog.h \
	colorpicker.h \
	controller.h \
	preferences.h \
	quicksettingsdialog.h \
	settingsdialog.h \
	toolbardialog.h \
	keysequenceedit.h

FORMS += \
	uploaddialog.ui \
	settingsdialog.ui

RESOURCES += \
	resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/shortcutkeybinder/release/ -lshortcutkeybinder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/shortcutkeybinder/debug/ -lshortcutkeybinder
else:unix: LIBS += -L$$OUT_PWD/../lib/shortcutkeybinder/ -lshortcutkeybinder

INCLUDEPATH += $$PWD/../lib/shortcutkeybinder
DEPENDPATH += $$PWD/../lib/shortcutkeybinder
