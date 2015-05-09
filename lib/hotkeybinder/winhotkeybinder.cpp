#include "winhotkeybinder.h"

WinHotKeyBinder* WinHotKeyBinder::m_pInstance = 0;

WinHotKeyBinder::WinHotKeyBinder(QObject *parent) :
    QObject(parent)
{
	m_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL,
	                                  keyboardProcedure,
	                                  GetModuleHandle(NULL),
	                                  0);

	atexit(&cleanUp);
}

WinHotKeyBinder::~WinHotKeyBinder()
{
	if(m_keyboardHook != NULL)
	{
		UnhookWindowsHookEx(m_keyboardHook);

		m_keyboardHook = NULL;
	}
}

void WinHotKeyBinder::cleanUp()
{
	delete m_pInstance;

	m_pInstance = 0;
}

LRESULT CALLBACK WinHotKeyBinder::keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		if(wParam == WM_KEYDOWN)
		{
			KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

			m_pInstance->m_activeKeys.insert(pKeyboard->vkCode);
		}
		else if(wParam == WM_KEYUP)
		{
			KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

			if(m_pInstance->m_activeKeys == m_pInstance->m_keys.toList().toSet())
			{
				Q_EMIT m_pInstance->hotKeyTriggered();
			}

			m_pInstance->m_activeKeys.remove(pKeyboard->vkCode);
		}
	}

	return false;
}

WinHotKeyBinder& WinHotKeyBinder::instance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new WinHotKeyBinder();
	}

	return *m_pInstance;
}

void WinHotKeyBinder::setHotKey(const HotKey& hotKey)
{
	m_keys = hotKey.keys();
}
