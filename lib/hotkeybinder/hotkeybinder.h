#ifndef HOTKEYBINDER_H
#define HOTKEYBINDER_H

// Qt includes
#include <QObject>

// Local includes
#include "hotkeybinder_global.h"

class QxtGlobalShortcut;

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
	void setKeySequence(const QKeySequence &keySequence);

	//! \brief Returns hot key as a string.
	//! \return Hot Key as a string.
	QKeySequence keySequence();

Q_SIGNALS:
	//! \brief Signal emitted when the hot key was triggered.
	void hotKeyTriggered();

private:
	QxtGlobalShortcut *m_pShortcut;
};

#endif // HOTKEYBINDER_H
