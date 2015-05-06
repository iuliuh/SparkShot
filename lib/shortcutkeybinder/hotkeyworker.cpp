#include "hotkeyworker.h"

#include <QMutexLocker>

HotKeyWorker::HotKeyWorker(QObject *parent) :
    QObject(parent)
{
	m_pDisplay = XOpenDisplay(0);
	m_grabWindow = DefaultRootWindow(m_pDisplay);
	m_modifiers = ControlMask | ShiftMask;
	m_keyCode = XKeysymToKeycode(m_pDisplay, XK_K);

	XGrabKey(m_pDisplay,
	         m_keyCode,
	         m_modifiers,
	         m_grabWindow,
	         False,
	         GrabModeAsync,
	         GrabModeAsync);

	XSelectInput(m_pDisplay, m_grabWindow, KeyPressMask);

	m_handleNextEvent = true;
}

HotKeyWorker::~HotKeyWorker()
{
	XCloseDisplay(m_pDisplay);

	delete m_pDisplay;
}

void HotKeyWorker::setHotKey(uint modifiers, int keyCode)
{
	m_handleNextEvent = false;

	XEvent unlockEvent;
	memset(&unlockEvent, 0, sizeof(XClientMessageEvent));
	unlockEvent.type = KeyPress;
	XSendEvent(m_pDisplay, m_grabWindow, 1, KeyPressMask, &unlockEvent);
	XFlush(m_pDisplay);

	m_mutex.lock();

	XUngrabKey(m_pDisplay, m_keyCode, m_modifiers, m_grabWindow);

	m_modifiers = modifiers;
	m_keyCode = XKeysymToKeycode(m_pDisplay, keyCode);

	XGrabKey(m_pDisplay,
	         m_keyCode,
	         m_modifiers,
	         m_grabWindow,
	         False,
	         GrabModeAsync,
	         GrabModeAsync);

	XSelectInput(m_pDisplay, m_grabWindow, KeyPressMask);

	m_handleNextEvent = true;

	m_mutex.unlock();
}

void HotKeyWorker::listen()
{
	XEvent ev;

	Q_FOREVER
	{
		QMutexLocker locker(&m_mutex);

		if(!m_handleNextEvent)
		{
			continue;
		}

		XNextEvent(m_pDisplay, &ev);

		if(ev.type == KeyPress)
		{
			qDebug() << "Hot key pressed!";
		}
	}
}

