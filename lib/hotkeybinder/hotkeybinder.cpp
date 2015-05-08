#include "hotkeybinder.h"

HotKeyBinder::HotKeyBinder(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_WIN
	m_pWinHotKeyBinder = new WinHotKeyBinder(this);

	connect(m_pWinHotKeyBinder, &WinHotKeyBinder::hotKeyTriggered,
	        this, &HotKeyBinder::hotKeyTriggered);
#endif

#ifdef Q_OS_LINUX
	m_pGNULinuxHotBinder = new GNULinuxHotKeyBinder(this);

	connect(m_pGNULinuxHotBinder, &GNULinuxHotKeyBinder::hotKeyTriggered,
	        this, &HotKeyBinder::hotKeyTriggered);
#endif
}

HotKeyBinder::~HotKeyBinder()
{
}

void HotKeyBinder::setHotKey(const QString& keySequence)
{
#ifdef Q_OS_WIN
	m_pWinHotKeyBinder->setHotKey(keySequence);
#endif

#ifdef Q_OS_LINUX
	m_pGNULinuxHotBinder->setHotKey(keySequence);
#endif
}

