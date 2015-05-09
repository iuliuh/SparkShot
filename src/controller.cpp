#include "controller.h"
#include "drawingboard.h"
#include "settingsdialog.h"
#include "splashscreen.h"
#include "hotkeybinder.h"

#include <QApplication>
#include <QTranslator>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QMessageBox>

Controller::Controller(QObject *pParent) : QObject(pParent)
{
	loadTranslator();

	m_pHotKeyBinder = new HotKeyBinder;
	m_pHotKeyBinder->setHotKey(Preferences::instance().hotKey());

	m_pSystemTray = new QSystemTrayIcon(QIcon(":/images/trayIconLight"), this);
	m_systemTrayMenu = new QMenu;

	m_printScreenAction = m_systemTrayMenu->addAction(tr("Print screen"));
	m_settingsAction = m_systemTrayMenu->addAction(tr("Settings"));
	m_systemTrayMenu->addSeparator();
	m_aboutAction = m_systemTrayMenu->addAction(tr("About"));
	m_systemTrayMenu->addSeparator();
	m_closeAction = m_systemTrayMenu->addAction(tr("Close"));

	m_pSystemTray->setContextMenu(m_systemTrayMenu);

	connect(m_pSystemTray, &QSystemTrayIcon::activated,
	        this, &Controller::onSystemTrayIconActivated);
	connect(m_printScreenAction, &QAction::triggered,
	        this, &Controller::onPrintScreenActionClicked);
	connect(m_settingsAction, &QAction::triggered,
	        this, &Controller::onSettingsActionClicked);
	connect(m_closeAction, &QAction::triggered,
	        qApp, &QApplication::exit);
	connect(m_aboutAction, &QAction::triggered,
	        this, &Controller::onAboutActionClicked);
	connect(m_pHotKeyBinder, &HotKeyBinder::hotKeyTriggered,
	        this, &Controller::onHotKeyActivated);
}

Controller::~Controller()
{
}

void Controller::start()
{
	m_pSystemTray->show();
}

void Controller ::onAboutActionClicked()
{
	m_pSplashScreen = new SplashScreen;
	m_pSplashScreen->show();
}

void Controller::onHotKeyChanged(const QString& hotKey)
{
	if(!m_pHotKeyBinder->setHotKey(hotKey))
	{
		QMessageBox msgBox;

		msgBox.setWindowTitle(tr("Invalid Hot Key"));
		msgBox.setText(tr("Invalid Hot Key"));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setInformativeText(tr("The Hot Key you are about to "
		                             "use is currently not supported, "
		                             "please try another one."));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);

		msgBox.exec();

		m_pSettingsDialog->setHotKey(Preferences::instance().hotKey());

		return;
	}

	Preferences::instance().setHotKey(hotKey);
}

void Controller::onHotKeyActivated()
{
	printScreen();
}

void Controller::loadTranslator()
{
	m_pTranslator = new QTranslator(this);

	if(Preferences::instance().language() == "Romana")
	{
		m_pTranslator->load(":/translations/romanian");
	}

	qApp->installTranslator(m_pTranslator);
}

void Controller::onPrintScreenActionClicked()
{
	printScreen();
}

void Controller::onSettingsActionClicked()
{
	m_pSettingsDialog = new SettingsDialog;

	connect(m_pSettingsDialog, &SettingsDialog::keySequenceChanged,
	        this, &Controller::onHotKeyChanged);

	m_pSettingsDialog->show();
}

void Controller::onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger)
	{
		printScreen();
	}
}

void Controller::printScreen()
{
	m_pDrawingBoard = new DrawingBoard;

	QTimer::singleShot(50, m_pDrawingBoard, &DrawingBoard::shoot);
}
