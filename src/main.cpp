#include <QApplication>
#include <QTranslator>

#include "controller.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setQuitOnLastWindowClosed(false);
	app.setOrganizationName("Devtel Software");
	app.setOrganizationDomain("http://www.devtelsoftware.com");
	app.setApplicationName("SparkShot");

	Controller controller;
	controller.start();

	return app.exec();
}
