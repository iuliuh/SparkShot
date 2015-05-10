#include "gnulinuxhotkeyworker.h"

#include <QMutexLocker>

GNULinuxHotKeyWorker::GNULinuxHotKeyWorker(QObject *pParent) :
    QObject(pParent),
    m_supressSignalEmition(0)
{
	// Initialize the resources
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

	m_handleNextEvent = 1;
}

GNULinuxHotKeyWorker::~GNULinuxHotKeyWorker()
{
	XCloseDisplay(m_pDisplay);

	free(m_pDisplay);
}

void GNULinuxHotKeyWorker::setHotKey(uint modifiers, int keyCode)
{
	m_supressSignalEmition = 1;
	m_handleNextEvent = 0;

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

	m_handleNextEvent = 1;

	m_mutex.unlock();
}

void GNULinuxHotKeyWorker::listen()
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

		if(ev.type == KeyPress && !m_supressSignalEmition)
		{
			Q_EMIT hotKeyTriggered();
		}
		else
		{
			m_supressSignalEmition = 0;
		}
	}
}

