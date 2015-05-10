#include "singleapplication.h"
#include "controller.h"

#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QTranslator>
#include <QIcon>
#include <QUuid>

int main(int argc, char *argv[])
{
	SingleApplication app(argc, argv, QUuid::createUuid().toString());

	SingleApplication::setWindowIcon(QIcon(":/images/applicationLogo"));

	if(app.alreadyExists())
	{
		qDebug() << "SparkShot is already running.";

		QMessageBox alreadyRunningMessageBox;

		alreadyRunningMessageBox.setWindowTitle(QObject::tr("SparkShot already running"));
		alreadyRunningMessageBox.setText(QObject::tr("SparkShot already running"));
		alreadyRunningMessageBox.setIcon(QMessageBox::Information);
		alreadyRunningMessageBox.setInformativeText(QObject::tr("One instance of the application "
		                                                        "is already running. Only one "
		                                                        "instance of SparkShot can run at "
		                                                        "a time."));
		alreadyRunningMessageBox.setStandardButtons(QMessageBox::Ok);
		alreadyRunningMessageBox.setDefaultButton(QMessageBox::Ok);

		alreadyRunningMessageBox.exec();

		return 0;
	}

	app.setQuitOnLastWindowClosed(false);
	app.setOrganizationName("Devtel Software");
	app.setOrganizationDomain("http://www.devtelsoftware.com");
	app.setApplicationName("SparkShot");

	Controller controller;
	controller.start();

	return app.exec();
}
