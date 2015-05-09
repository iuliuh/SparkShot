#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSystemTrayIcon>

class QTranslator;
class QMenu;
class QAction;
class DrawingBoard;
class SettingsDialog;
class SplashScreen;
class HotKeyBinder;

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
	void onHotKeyChanged(const QString& hotKey);
	void onHotKeyActivated();

private:
	void loadTranslator();
	void printScreen();

private:
	DrawingBoard *m_pDrawingBoard;
	SettingsDialog *m_pSettingsDialog;
	SplashScreen *m_pSplashScreen;
	HotKeyBinder *m_pHotKeyBinder;

	QSystemTrayIcon *m_pSystemTray;
	QTranslator *m_pTranslator;

	QAction *m_printScreenAction;
	QAction *m_settingsAction;
	QAction *m_closeAction;
	QAction *m_aboutAction;

	QMenu *m_systemTrayMenu;
};

#endif // CONTROLLER_H
