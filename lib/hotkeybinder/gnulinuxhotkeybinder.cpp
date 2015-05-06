#include "gnulinuxhotkeybinder.h"

#include <QThread>

GNULinuxHotKeyBinder::GNULinuxHotKeyBinder(QObject *parent) :
    QObject(parent)
{
	m_pHotKeyWorker = new HotKeyWorker;
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

void GNULinuxHotKeyBinder::setKeySequence(const QString &keySequence)
{
	m_pHotKeyWorker->setHotKey(ControlMask | ShiftMask, XK_Y);
}
