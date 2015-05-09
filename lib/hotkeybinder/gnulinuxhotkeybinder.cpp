#include "gnulinuxhotkeybinder.h"

#include <QThread>

GNULinuxHotKeyBinder::GNULinuxHotKeyBinder(QObject *pParent) :
    QObject(pParent)
{
	// Create a worker object
	m_pHotKeyWorker = new GNULinuxHotKeyWorker;

	// Do the connections between this and the worker object which
	// will be listening to hot keys from another thread. There is
	// need to do the listening in another thread beacause the
	// listening blocks the main thread.
	connect(m_pHotKeyWorker, &GNULinuxHotKeyWorker::hotKeyTriggered,
	        this, &GNULinuxHotKeyBinder::hotKeyTriggered);

	// Create the new thread handler
	m_pHotKeyThread = new QThread;

	// Change the worker object thread affinity
	m_pHotKeyWorker->moveToThread(m_pHotKeyThread);

	// Start the thread
	m_pHotKeyThread->start();

	// Invoke the GNULinuxHotKeyWorker::listen function from the
	// new thread event loop
	QMetaObject::invokeMethod(m_pHotKeyWorker, "listen");
}

GNULinuxHotKeyBinder::~GNULinuxHotKeyBinder()
{
}

void GNULinuxHotKeyBinder::setHotKey(const QString& hotKey)
{
	m_pHotKeyWorker->setHotKey(ControlMask | ShiftMask, XK_Y);
}
