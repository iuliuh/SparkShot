#ifndef HOTKEY_H
#define HOTKEY_H

#include "hotkeybinder_global.h"

#include <QtGlobal>
#include <QVector>
#include <QMap>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

//! \brief Class represents a hot key or a key sequence. A HotKey
//!        should be created from a string using HotKey::fromString
//!        member function. It supports formats such as Ctrl+A,
//!        Ctrl+Shift+P,  Alt+Shift+Space, etc.
class HOTKEYBINDERSHARED_EXPORT HotKey
{
public:
	//! \brief Constructts a HotKey.
	HotKey();

	//! \brief Destroys the HotKey.
	~HotKey();

	//! \brief Checks if HotKey is valid.
	//! \return Returns true if HotKey is valid, false otherwise.
	bool isValid() const;

#ifdef Q_OS_WIN
	//! \brief Provides the keys which form the hot key.
	//! \return Returns the keys which form the hot key.
	QVector<DWORD> keys() const;

	//! \brief Provides the key sequence number of keys.
	//! \return Returns the number of keys in the hot key. For
	//!         example if key sequence is Ctrl+P, function
	//!         returns 2. If sequence is Ctrl+Alt+B, function
	//!         returns 3. If HotKey is invalid it will return 0.
	int count() const;
#endif

	//! \brief Utility function for converting a string to a HotKey.
	//!        HotKey supports formats such as Ctrl+P or Ctrl+Shift+T.
	static HotKey fromString(const QString& hotKeyString);

private:
	//! \brief Defines wether or not the hot key is a valid hot
	//!        key or an invalid one.
	//! \param valid True if the hotkey is valid, false otherwise.
	void setValid(bool valid);

	//! \brief Initializes a map between the pressed keys as a
	//!        string and their corresponding OS key codes.
	void initializeHotKeyMap();

private:
#ifdef Q_OS_WIN
	QVector<DWORD> m_keys;
	QMap<QString, DWORD> m_keyMap;
#endif

	bool m_valid;
};

#endif // HOTKEY_H
