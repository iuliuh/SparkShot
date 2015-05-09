#ifndef HOTKEYBINDER_H
#define HOTKEYBINDER_H

// Qt includes
#include <QObject>

// Local includes
#include "hotkeybinder_global.h"

#ifdef Q_OS_LINUX
#include "gnulinuxhotkeybinder.h"
#endif

#ifdef Q_OS_WIN
#include "winhotkeybinder.h"
#endif

//! \brief Class handles a hot key bind. A hot key is set with
//!        HotKeyBinder::setHotKey and when the user hits the
//!        defined key sequence HotKeyBinder::hotKeyTriggered
//!        signal is emitted.
class HOTKEYBINDERSHARED_EXPORT HotKeyBinder : public QObject
{
	Q_OBJECT

public:
	//! \brief Constructs a HotKeyBinder object.
	explicit HotKeyBinder(QObject *parent = 0);

	//! \brief Destroys a HotKeyBinder object.
	~HotKeyBinder();

	//! \brief Sets a hot key.
	//! \param keySequence The hot key to be set. Supports formats
	//!        such as Ctrl+P, Ctrl+Alt+L, Meta+M, etc.
	bool setHotKey(const QString& keySequence);

	//! \brief Returns hot key as a string.
	//! \return Hot Key as a string.
	QString toString();

Q_SIGNALS:
	//! \brief Signal emitted when the hot key was triggered.
	void hotKeyTriggered();

private:
	// Hot key
	QString m_keySequence;

#ifdef Q_OS_WIN
	// Windows key binder
	WinHotKeyBinder* m_pWinHotKeyBinder;
#endif

#ifdef Q_OS_LINUX
	// GNU/Linux key binder
	GNULinuxHotKeyBinder* m_pGNULinuxHotBinder;
#endif
};

#endif // HOTKEYBINDER_H
