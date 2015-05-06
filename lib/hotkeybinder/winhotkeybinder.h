#ifndef WINHOTKEYBINDER_H
#define WINHOTKEYBINDER_H

#include <QObject>

#include <Windows.h>

class WinHotKeyBinder : public QObject
{
	Q_OBJECT

public:
	WinHotKeyBinder(QObject *pParent = 0);

	~WinHotKeyBinder();

	void setHotKey(DWORD modifiers, DWORD key);

Q_SIGNALS:
	void hotKeyTriggered();

private:
	static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam);

private:
	HHOOK m_keyboardHook;

	DWORD m_key;
	DWORD m_modifiers;
};

#endif // WINHOTKEYBINDER_H
