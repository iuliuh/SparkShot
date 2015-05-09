#ifndef WINHOTKEYBINDER_H
#define WINHOTKEYBINDER_H

// Qt includes
#include <QObject>
#include <QString>
#include <QVector>
#include <QSet>
#include <QList>

// Windows includes
#include <Windows.h>

// Local includes
#include "hotkey.h"

//! \brief Hot Key binder singleton class for MS Windows. Listens for the user
//!        to hit the hot key specified with WinHotKeyBinder::setHotKey
//!        and emits the WinHotKeyBinder::hotKeyTriggered signal when the
//!        hot key was triggered.
class WinHotKeyBinder : public QObject
{
	Q_OBJECT

public:
	//! \brief Provides a WinHotKeyBinder instance. If instance is not created
	//!        it automatically creates one.
	//! \return WinHotKeyBinder reference.
	static WinHotKeyBinder& instance();

public Q_SLOTS:
	//! \brief Sets the hot key to listen to.
	//! \param hotKey Hot key to listen to. Supports formats such as Ctrl+P,
	//!               Ctrl+T, Shift+Alt+B, etc.
	//! \see WinHotKeyBinder::hotKeyTriggered.
	void setHotKey(const HotKey& hotKey);

Q_SIGNALS:
	//! \brief Signal emitted when the hot key set with
	//!        GNULinuxHotKeyBinder::setHotKey was set successfully.
	void hotKeyTriggered();

private:
	//! \brief Constructs a GNULinuxHotKeyBinder object.
	//! \brief pParent Parent object.
	WinHotKeyBinder(QObject* pParent = 0);

	//! \brief Destroys a GNULinuxHotKeyBinder object.
	~WinHotKeyBinder();

	//! \brief Disable copying.
	Q_DISABLE_COPY(WinHotKeyBinder)

	//! \brief Frees the allocated resources.
	static void cleanUp();

	//! \brief Function called when a key gets pressed.
	//! \see HOOKPROC for more info.
	static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam);

private:
	// Instance
	static WinHotKeyBinder* m_pInstance;

	// Keyboard hook
	HHOOK m_keyboardHook;

	// Keys representing the binding to listen to
	QVector<DWORD> m_keys;

	// Currently pressed keys
	QSet<DWORD> m_activeKeys;
};

#endif // WINHOTKEYBINDER_H
