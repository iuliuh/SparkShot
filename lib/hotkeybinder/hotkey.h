#ifndef HOTKEY_H
#define HOTKEY_H

#include "hotkeybinder_global.h"

#include <QtGlobal>
#include <QVector>
#include <QMap>

#include <Windows.h>

class HOTKEYBINDERSHARED_EXPORT HotKey
{
public:
	HotKey();
	~HotKey();

	bool isValid() const;

#ifdef Q_OS_WIN
	QVector<DWORD> keys() const;
	int count() const;
#endif

	static HotKey fromString(const QString& hotKeyString);

private:
	void setValid(bool valid);
	void initializeHotKeyMap();

private:
#ifdef Q_OS_WIN
	QVector<DWORD> m_keys;
	QMap<QString, DWORD> m_keyMap;
#endif

	bool m_valid;
};

#endif // HOTKEY_H
