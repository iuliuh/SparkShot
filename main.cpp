#include <QApplication>
#include "screenshotarea.h"
#include "toolbar.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ScreenshotArea s;
	s.shoot();

//	ToolBar t;
//	t.show();

	return a.exec();
}
