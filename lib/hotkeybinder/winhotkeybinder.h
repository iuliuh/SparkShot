#ifndef WINHOTKEYBINDER_H
#define WINHOTKEYBINDER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QSet>
#include <QList>

#include <Windows.h>

#include "hotkey.h"

class WinHotKeyBinder : public QObject
{
	Q_OBJECT

public:
	static WinHotKeyBinder& instance();

	void setHotKey(const HotKey& hotKey);

Q_SIGNALS:
	void hotKeyTriggered();

private:
	WinHotKeyBinder(QObject* pParent = 0);
	~WinHotKeyBinder();

	Q_DISABLE_COPY(WinHotKeyBinder)

	static void cleanUp();

	static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam);

private:
	static WinHotKeyBinder* m_pInstance;

	HHOOK m_keyboardHook;

	QVector<DWORD> m_keys;
	QSet<DWORD> m_activeKeys;
};

#endif // WINHOTKEYBINDER_H
