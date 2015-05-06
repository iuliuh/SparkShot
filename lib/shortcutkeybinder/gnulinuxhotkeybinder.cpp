#include "hotkeybinder.h"

#include "hotkeyworker.h"
#include <QThread>

HotKeyBinder::HotKeyBinder(QObject *parent) : QObject(parent)
{
	m_pHotKeyWorker = new HotKeyWorker;
	m_pHotKeyThread = new QThread;

	m_pHotKeyWorker->moveToThread(m_pHotKeyThread);
	m_pHotKeyThread->start();

	QMetaObject::invokeMethod(m_pHotKeyWorker, "listen");
}

HotKeyBinder::~HotKeyBinder()
{
}

QKeySequence HotKeyBinder::keySequence() const
{
	return m_keySequence;
}

void HotKeyBinder::setKeySequence(const QString &keySequence)
{
//	m_keySequence = keySequence;

	// Convert keys here

	m_pHotKeyWorker->setHotKey(ControlMask | ShiftMask, XK_Y);
}
