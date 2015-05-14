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
	keysequenceedit.cpp \
	splashscreen.cpp \
	splashscreenitem.cpp \
	singleapplication.cpp

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
	keysequenceedit.h \
	splashscreen.h \
	splashscreenitem.h \
	singleapplication.h

FORMS += \
	uploaddialog.ui \
	settingsdialog.ui

RESOURCES += \
	../resources/resources.qrc

TRANSLATIONS += \
	../resources/translations/romanian.ts

DOXYGEN_BIN = ""

win32 {
	DOXYGEN_BIN = $$system(where doxygen)
}

unix {
	DOXYGEN_BIN = $$system(which doxygen)
}

isEmpty(DOXYGEN_BIN) {
	message("Doxygen was not found in your system. Please install doxygen to generate the docs.")
} else {
	docs.commands = doxygen $$PWD/../docs/Doxyfile

	QMAKE_EXTRA_TARGETS += docs
	POST_TARGETDEPS += docs
}

# Deploy
win32 {
	CONFIG(release, debug|release) {
		QMAKE_POST_LINK = cp $$OUT_PWD/"release/"$$TARGET".exe" $$PWD/../deploy/win/packages/com.devtelsoftware.sparkshot/data
	}
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/hotkeybinder/release/ -lhotkeybinder -lole32 -loleaut32 -luuid
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/hotkeybinder/debug/ -lhotkeybinder
else:unix: LIBS += -L$$OUT_PWD/../lib/hotkeybinder/ -lhotkeybinder

INCLUDEPATH += $$PWD/../lib/hotkeybinder
DEPENDPATH += $$PWD/../lib/hotkeybinder
