#include <QApplication>
#include "screen.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ScreenshotArea s;
	s.shoot();

	return a.exec();
}
