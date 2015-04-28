#include "controller.h"
#include "drawingboard.h"

#include <QMenu>
#include <QAction>

#include <QDebug>

Controller::Controller(QObject *parent) : QObject(parent)
{
	m_systemTray = new QSystemTrayIcon(QIcon(":/images/trayIcon"), this);
	m_drawingBoard = new DrawingBoard;
	m_systemTrayMenu = new QMenu;

	m_printScreenAction = m_systemTrayMenu->addAction(tr("Print Screen!"));
	m_closeAction = m_systemTrayMenu->addAction(tr("Close"));
	m_systemTrayMenu->addSeparator();
	m_aboutAction = m_systemTrayMenu->addAction(tr("About"));

	m_systemTray->setContextMenu(m_systemTrayMenu);

	connect(m_aboutAction, SIGNAL(triggered()),
	        this, SLOT(onAboutActionClicked()));
	connect(m_printScreenAction, SIGNAL(triggered()),
	        this, SLOT(onPrintScreenActionClicked()));
	connect(m_closeAction, SIGNAL(triggered()),
	        this, SLOT(onCloseActionClicked()));
	connect(m_systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	        this, SLOT(onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason)));
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

void Controller::onCloseActionClicked()
{
	qApp->quit();
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
	m_drawingBoard->shoot();
}
