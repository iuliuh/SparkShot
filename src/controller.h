#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSystemTrayIcon>

class QMenu;
class DrawingBoard;
class QAction;
class SettingsDialog;

class Controller : public QObject
{
	Q_OBJECT

public:
	explicit Controller(QObject *parent = 0);
	~Controller();

	void start();

private Q_SLOTS:
	void onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void onPrintScreenActionClicked();
	void onSettingsActionClicked();
	void onAboutActionClicked();

private:
	void printScreen();

private:
	DrawingBoard *m_pDrawingBoard;
	SettingsDialog *m_pSettingsDialog;

	QSystemTrayIcon *m_systemTray;

	QAction *m_printScreenAction;
	QAction *m_settingsAction;
	QAction *m_closeAction;
	QAction *m_aboutAction;

	QMenu *m_systemTrayMenu;
};

#endif // CONTROLLER_H
