#include "gnulinuxhotkeybinder.h"

#include <QThread>

GNULinuxHotKeyBinder::GNULinuxHotKeyBinder(QObject *parent) :
    QObject(parent)
{
	m_pHotKeyWorker = new GNULinuxHotKeyWorker;
	m_pHotKeyThread = new QThread;

	m_pHotKeyWorker->moveToThread(m_pHotKeyThread);

	m_pHotKeyThread->start();

	QMetaObject::invokeMethod(m_pHotKeyWorker, "listen");
}

GNULinuxHotKeyBinder::~GNULinuxHotKeyBinder()
{
}

QKeySequence GNULinuxHotKeyBinder::keySequence() const
{
	return m_keySequence;
}

void GNULinuxHotKeyBinder::setHotKey(uint modifiers, uint key)
{
	m_pHotKeyWorker->setHotKey(ControlMask | ShiftMask, XK_Y);
}
