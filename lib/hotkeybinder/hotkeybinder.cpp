#include "hotkeybinder.h"
#include "hotkey.h"

HotKeyBinder::HotKeyBinder(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_WIN
	m_pWinHotKeyBinder = &WinHotKeyBinder::instance();

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

bool HotKeyBinder::setHotKey(const QString& keySequence)
{
	HotKey hotKey = HotKey::fromString(keySequence);

	if(!hotKey.isValid())
	{
		return false;
	}

	m_keySequence = keySequence;

#ifdef Q_OS_WIN
	m_pWinHotKeyBinder->setHotKey(hotKey);
#endif

#ifdef Q_OS_LINUX
	m_pGNULinuxHotBinder->setHotKey(hotKey);
#endif

	return true;
}

QString HotKeyBinder::toString()
{
	return m_keySequence;
}

