#include <QApplication>

#include "drawingboard.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DrawingBoard s;
	s.shoot();

	return a.exec();
}
