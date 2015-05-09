#ifndef CONTROLLER_H
#define CONTROLLER_H

// Qt includes
#include <QObject>
#include <QSystemTrayIcon>

// Forward declarations
class QTranslator;
class QMenu;
class QAction;
class DrawingBoard;
class SettingsDialog;
class SplashScreen;
class HotKeyBinder;

//! \brief Manages the whole application
class Controller : public QObject
{
	Q_OBJECT

public:
	//! \brief Constructs a Controller object.
	//! \brief pParent Parent object.
	explicit Controller(QObject *pParent = 0);

	//! \brief Destroys a Controller object.
	~Controller();

	//! \brief Starts application execution.
	void start();

private Q_SLOTS:
	//! \brief Slot called when the system tray icon action gets activated.
	//! \param reason Activation reason.
	//! \see QSystemTrayIcon::ActivationReason
	void onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

	//! \brief Slot called when the print screen system tray action was clicked.
	void onPrintScreenActionClicked();

	//! \brief Slot called when the settings tray action was clicked.
	void onSettingsActionClicked();

	//! \brief Slot called when the about tray action was clicked.
	void onAboutActionClicked();

	//! \brief Slot called when the hot key gets changed.
	void onHotKeyChanged(const QString& hotKey);

	//! \brief Slot called when the hot key was triggered.
	void onHotKeyActivated();

private:
	//! \brief Loads the application translator.
	void loadTranslator();

	//! \brief Triggers a print screen.
	void printScreen();

private:
	// Drawing board. This is where everything gets painted
	DrawingBoard *m_pDrawingBoard;

	// Settings dialog
	SettingsDialog *m_pSettingsDialog;

	// Splash screen
	SplashScreen *m_pSplashScreen;

	// Hot key binder
	HotKeyBinder *m_pHotKeyBinder;

	// Tray icon
	QSystemTrayIcon *m_pSystemTray;

	// Translator
	QTranslator *m_pTranslator;

	// Tray icon actions
	QAction *m_printScreenAction;
	QAction *m_settingsAction;
	QAction *m_closeAction;
	QAction *m_aboutAction;

	// Tray icon menu
	QMenu *m_systemTrayMenu;
};

#endif // CONTROLLER_H
