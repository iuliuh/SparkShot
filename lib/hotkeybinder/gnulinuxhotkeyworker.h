#ifndef HOTKEYWORKER_H
#define HOTKEYWORKER_H

// Qt includes
#include <QObject>
#include <QMutex>
#include <QDebug>
#include <QAtomicInt>

// X11 includes
#include <X11/Xlib.h>
#include <X11/Xutil.h>

//! \brief Class used to execute the task of listening to hot keys
//!        in another thread. There is need to do the listening in
//!        another thread because waiting to get notified by a key
//!        press by the X11 API blocks and we can't block the GUI
//!        thread.
class GNULinuxHotKeyWorker : public QObject
{
	Q_OBJECT

public:
	//! \brief Constructs a GNULinuxHotKeyWorker object.
	//! \param pParent Parent object.
	explicit GNULinuxHotKeyWorker(QObject *pParent = 0);

	//! \brief Destroys the GNULinuxHotKeyWorker object.
	~GNULinuxHotKeyWorker();

	//! \brief Set the hot key to listen to
	//! \param modifiers The modifiers which consist of a bit-mask
	//!                  constructred from ControlMask, ShiftMask,
	//!                  etc.
	//! \param keyCode The key to be pressed, e.g. XK_K, XK_Y, etc.
	void setHotKey(uint modifiers, int keyCode);

public Q_SLOTS:
	//! \brief Starts an in finite loop which listens for hot keys to be
	//!        pressed and emits GNULinuxHotKeyWorker::hotKeyTriggered
	//!        when a hot key set with GNULinuxHotKeyWorker::setHotKey
	//!        is pressed.
	void listen();

Q_SIGNALS:
	//! \brief Signal emitted when the hot key set with
	//!        GNULinuxHotKeyWorker::setHotKey is triggered.
	void hotKeyTriggered();

private:
	// Synchronisation mutex
	QMutex m_mutex;

	// X11 display
	Display* m_pDisplay;

	// X11 window
	Window m_grabWindow;

	// Modifiers
	uint m_modifiers;

	// Key
	int m_keyCode;

	// Breaks the infinite loop when the hot key changes
	// 1 - next event should be handeld, 0 - otherwise
	QAtomicInt m_handleNextEvent;

	// Blocks signal emition. Usefull when the dummy event
	// is sent in order to exit the infinite loop and change
	// the hotkey. 1 - supress, 0 - don't supress
	QAtomicInt m_supressSignalEmition;
};

#endif // HOTKEYWORKER_H
