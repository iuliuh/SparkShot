#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSystemTrayIcon>

class QMenu;
class DrawingBoard;
class QAction;

class Controller : public QObject
{
	Q_OBJECT

public:
	explicit Controller(QObject *parent = 0);
	~Controller();

	void start();

private Q_SLOTS:
	void onAboutActionClicked();
	void onPrintScreenActionClicked();
	void onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	void printScreen();

private:
	DrawingBoard *m_drawingBoard;
	QSystemTrayIcon *m_systemTray;
	QMenu *m_systemTrayMenu;
	QAction *m_printScreenAction;
	QAction *m_closeAction;
	QAction *m_aboutAction;
};

#endif // CONTROLLER_H
