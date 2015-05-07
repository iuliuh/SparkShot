#ifndef HOTKEY_H
#define HOTKEY_H

#include "hotkeybinder_global.h"

#include <QtGlobal>

class HOTKEYBINDERSHARED_EXPORT HotKey
{
public:
	HotKey();
	~HotKey();

	bool valid() const;
	void setValid(bool valid);

#ifdef Q_OS_LINUX
	uint modifiers() const;
	void setModifiers(uint modifiers);

	uint key() const;
	void setKey(uint key);
#endif

#ifdef Q_OS_WIN
	DWORD modifiers() const;
	void setModifiers(DWORD modifiers);

	DWORD key() const;
	void setKey(DWORD key);
#endif

private:
	uint m_modifiers;
	uint m_key;
	bool m_valid;
};

#endif // HOTKEY_H
