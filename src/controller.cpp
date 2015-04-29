#include "controller.h"
#include "drawingboard.h"

#include <QMenu>
#include <QAction>
#include <QApplication>

Controller::Controller(QObject *parent) : QObject(parent)
{
	m_systemTray = new QSystemTrayIcon(QIcon(":/images/trayIcon"), this);
	m_systemTrayMenu = new QMenu;

	m_printScreenAction = m_systemTrayMenu->addAction(tr("Print Screen!"));
	m_closeAction = m_systemTrayMenu->addAction(tr("Close"));
	m_systemTrayMenu->addSeparator();
	m_aboutAction = m_systemTrayMenu->addAction(tr("About"));

	m_systemTray->setContextMenu(m_systemTrayMenu);

	connect(m_aboutAction, &QAction::triggered,
	        this, &Controller::onAboutActionClicked);
	connect(m_printScreenAction, &QAction::triggered,
	        this, &Controller::onPrintScreenActionClicked);
	connect(m_closeAction, &QAction::triggered,
	        qApp, &QApplication::exit);
	connect(m_systemTray, &QSystemTrayIcon::activated,
	        this, &Controller::onSystemTrayIconActivated);
}

Controller::~Controller()
{
	delete m_drawingBoard;
}

void Controller::start()
{
	m_systemTray->show();
}

void Controller::onAboutActionClicked()
{
	qDebug() << Q_FUNC_INFO;
}

void Controller::onPrintScreenActionClicked()
{
	printScreen();
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
	m_drawingBoard = new DrawingBoard;
	m_drawingBoard->shoot();
}
