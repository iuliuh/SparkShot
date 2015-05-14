#include "controller.h"
#include "drawingboard.h"
#include "settingsdialog.h"
#include "splashscreen.h"
#include "hotkeybinder.h"
#include "defines.h"

#include <QApplication>
#include <QTranslator>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QString>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <shlobj.h>
#endif

Controller::Controller(QObject *pParent) : QObject(pParent)
{
	m_settingsDialogIsDestroyed = true;

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
	        this, &Controller::onShowSettingsDialogActionClicked);
	connect(m_closeAction, &QAction::triggered,
	        qApp, &QApplication::exit);
	connect(m_aboutAction, &QAction::triggered,
	        this, &Controller::onAboutActionClicked);
	connect(m_pHotKeyBinder, &HotKeyBinder::hotKeyTriggered,
	        this, &Controller::onHotKeyActivated);

	if(Preferences::instance().isLaunchingOnSystemStartup())
	{
		const QString appDataPath = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
		QDir appDataDir(appDataPath);

		appDataDir.cd("../../../Roaming/Microsoft/Windows/Start Menu/Programs/Startup");

		if(appDataDir.exists())
		{
			const QString appDataDirShortcutPath = QDir::toNativeSeparators(appDataDir.absolutePath().append("/SparkShot.lnk"));
			const QString appExecutablePath = QApplication::arguments().at(0);

			createShortcut(appExecutablePath, appDataDirShortcutPath);
		}
	}
}

Controller::~Controller()
{
}

void Controller::start()
{
	if(Preferences::instance().isTrayIconShown())
	{
		m_pSystemTray->show();
	}
}

void Controller::onMessageAvailable(const QStringList &messages)
{
	Q_FOREACH(QString message, messages)
	{
		if(message == MESSAGE_SHOW_SETTINGS)
		{
			if(Preferences::instance().isTrayIconShown())
			{
				return;
			}

			if(m_settingsDialogIsDestroyed)
			{
				createSettingsDialog();
			}

			m_pSettingsDialog->show();
		}
	}
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

void Controller::onSettingsDialogDestroyed()
{
	m_settingsDialogIsDestroyed = true;
}

void Controller::onTrayIconShowStateChanged(bool state)
{
	m_pSystemTray->setVisible(state);
}

void Controller::onLaunchAtStartupStateChanged(bool state)
{
	const QString appDataPath = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
	QDir appDataDir(appDataPath);

	appDataDir.cd("../../../Roaming/Microsoft/Windows/Start Menu/Programs/Startup");

	const QString appDataDirShortcutPath = QDir::toNativeSeparators(appDataDir.absolutePath().append("/SparkShot.lnk"));
	const QString appExecutablePath = QApplication::arguments().at(0);

	if(appDataDir.exists())
	{
		if(state)
		{
			createShortcut(appExecutablePath, appDataDirShortcutPath);
		}
		else
		{
			QFile startupLink(appDataDirShortcutPath);
			startupLink.remove();
		}
	}
}

void Controller::createShortcut(const QString& filePath, const QString& shortcutPath)
{
#ifdef Q_OS_WIN
	CoInitialize(NULL);
	IShellLink* pShellLink = NULL;
	HRESULT hres = CoCreateInstance(CLSID_ShellLink,
									NULL,
									CLSCTX_ALL,
									IID_IShellLink,
									(void**)&pShellLink);

	if (SUCCEEDED(hres))
	{
		pShellLink->SetPath(filePath.toStdWString().c_str());
		pShellLink->SetDescription(L"SparkShot");
		pShellLink->SetIconLocation(filePath.toStdWString().c_str(), 0);

		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres))
		{
			hres = pPersistFile->Save(shortcutPath.toStdWString().c_str(), TRUE);

			pPersistFile->Release();
		}
		else
		{
			//cout << "Error 2" << endl;
			return;
		}
		pShellLink->Release();
	}
	else
	{
		//cout << "Error 1" << endl;
		return;
	}
#endif
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

void Controller::onShowSettingsDialogActionClicked()
{
	if(m_settingsDialogIsDestroyed)
	{
		createSettingsDialog();
	}

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

void Controller::createSettingsDialog()
{
	m_pSettingsDialog = new SettingsDialog;

	connect(m_pSettingsDialog, &SettingsDialog::keySequenceChanged,
	        this, &Controller::onHotKeyChanged);

	connect(m_pSettingsDialog, &SettingsDialog::destroyed,
	        this, &Controller::onSettingsDialogDestroyed);

	connect(m_pSettingsDialog, &SettingsDialog::showTrayIconChanged,
	        this, &Controller::onTrayIconShowStateChanged);

	connect(m_pSettingsDialog, &SettingsDialog::launchAtSystemStartupChanged,
	        this, &Controller::onLaunchAtStartupStateChanged);

	m_settingsDialogIsDestroyed = false;
}
