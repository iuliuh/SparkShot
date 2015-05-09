#include <QApplication>
#include <QTranslator>
#include <QIcon>

#include "controller.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QApplication::setWindowIcon(QIcon(":/images/applicationLogo"));

	app.setQuitOnLastWindowClosed(false);
	app.setOrganizationName("Devtel Software");
	app.setOrganizationDomain("http://www.devtelsoftware.com");
	app.setApplicationName("SparkShot");

	Controller controller;
	controller.start();

	return app.exec();
}
