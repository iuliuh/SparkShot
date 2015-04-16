#include <QApplication>

#include "screenshotarea.h"
#include "colorpicker.h"

/*
 * TODO:
 *
 *  1.0:
 *   - Color picker
 *   - Text drawing
 *   - Cropped area move/resize
 *   - Imgur upload
 *   - Preferences dialog
 *   - Toolbar auto out of screen
 *     autopositioning
 *   - Translations
 *   - Clean up: - includes
 *               - const correctness
 *               - comments
 *               - pointers/nonpointers
 *               - error checking on network,
 *                 data saving and json
 *               - respect a coding convention
 *
 *  2.0:
 *   - Undo
 *   - Photoshop-like tool buttons
 *   - No border around saved images
 *   - Multi-monitor support
 *   - Social networks share button
 *   - Clean up
 *
 *  3.0:
 *   - Theming support
 *   - Drawing plugins
 *   - Stabilization & bug fixing
 *   - Website
 *   - Clean up
 */

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ScreenshotArea s;
	s.shoot();

//	ColorPicker p;
//	p.resize(100, 100);
//	p.show();

	return a.exec();
}
