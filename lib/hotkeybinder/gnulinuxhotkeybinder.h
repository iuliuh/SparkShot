#ifndef GNULINUXHOTKEYBINDER_H
#define GNULINUXHOTKEYBINDER_H

// Qt includes
#include <QObject>
#include <QString>
#include <QKeySequence>

// Local includes
#include "gnulinuxhotkeyworker.h"

// Forward declarations
class GNULinuxHotKeyWorker;
class QThread;

//! \brief Hot Key binder for GNU/Linux. Listens for the user to hit the
//!        hot key specified with GNULinuxHotKeyBinder::setHotKey and
//!        emits the GNULinuxHotKeyBinder::hotKeyTriggered signal when the
//!        hot key was triggered.
class GNULinuxHotKeyBinder : public QObject
{
	Q_OBJECT

public:
	//! \brief Constructs a GNULinuxHotKeyBinder object.
	//! \brief pParent Parent object.
	GNULinuxHotKeyBinder(QObject *pParent = 0);

	//! \brief Destroys a GNULinuxHotKeyBinder object.
	~GNULinuxHotKeyBinder();

public Q_SLOTS:
	//! \brief Sets the hot key to listen to.
	//! \param hotKey Hot key to listen to. Supports formats such as Ctrl+P,
	//!               Ctrl+T, Shift+Alt+B, etc.
	//! \see GNULinuxHotKeyBinder::hotKeyTriggered.
	void setHotKey(const QString& hotKey);

Q_SIGNALS:
	//! \brief Signal emitted when the hot key set with
	//!        GNULinuxHotKeyBinder::setHotKey was set successfully.
	void hotKeyTriggered();

private:
	// Hot key worker object.
	GNULinuxHotKeyWorker* m_pHotKeyWorker;

	// Thread in which a GNULinuxHotKeyWorker is executed.
	QThread* m_pHotKeyThread;
};

#endif // GNULINUXHOTKEYBINDER_H
