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
#endif
}

HotKeyBinder::~HotKeyBinder()
{
}

void HotKeyBinder::setHotKey(const QString& keySequence)
{
	//! \todo Convert QString to (uint modifiers, uint key)
	Q_UNUSED(keySequence)
	uint modifiers;
	uint key;

#ifdef Q_OS_WIN
	m_pWinHotKeyBinder->setHotKey(modifiers, key);
#endif

#ifdef Q_OS_LINUX
	m_pGNULinuxHotBinder->setHotKey(modifiers, key);
#endif
}

