#include "singleapplication.h"
#include "controller.h"
#include "defines.h"

#include <QObject>
#include <QMessageBox>
#include <QTranslator>
#include <QIcon>

int main(int argc, char *argv[])
{
	SingleApplication app(argc, argv, APPLICATION_KEY);

	SingleApplication::setWindowIcon(QIcon(":/images/applicationLogo"));

	if(app.alreadyExists())
	{
		Q_EMIT app.sendMessage(MESSAGE_SHOW_SETTINGS);

		return 0;
	}

	app.setQuitOnLastWindowClosed(false);
	app.setOrganizationName("Devtel Software");
	app.setOrganizationDomain("http://www.devtelsoftware.com");
	app.setApplicationName("SparkShot");

	Controller controller;

	QObject::connect(&app, &SingleApplication::messageAvailable,
	                 &controller, &Controller::onMessageAvailable);

	controller.start();

	return app.exec();
}
