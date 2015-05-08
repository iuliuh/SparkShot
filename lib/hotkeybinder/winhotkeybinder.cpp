#include "winhotkeybinder.h"

#include <QDebug>

WinHotKeyBinder::WinHotKeyBinder(QObject *parent) :
    QObject(parent)
{
	m_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL,
	                                  keyboardProcedure,
	                                  GetModuleHandle(NULL),
	                                  0);
}

WinHotKeyBinder::~WinHotKeyBinder()
{
	if(m_keyboardHook != NULL)
	{
		UnhookWindowsHookEx(m_keyboardHook);

		m_keyboardHook = NULL;
	}
}

LRESULT CALLBACK WinHotKeyBinder::keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
	// Emit hotKeyTriggered here
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_KEYDOWN)
		{
			KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

			qDebug() << "WM_KEYDOWN: " << pKeyboard->vkCode;
		}
		else if (wParam == WM_KEYUP)
		{
			KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

			qDebug() << "WM_KEYUP: " << pKeyboard->vkCode;
		}
	}

	return false;
}

void WinHotKeyBinder::setHotKey(const QString& hotKey)
{
//	m_modifiers = hotKey;
//	m_key = key;
	qDebug() << hotKey;
}
