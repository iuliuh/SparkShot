#ifndef HOTKEYWORKER_H
#define HOTKEYWORKER_H

#include <QObject>
#include <QMutex>
#include <QDebug>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class HotKeyWorker : public QObject
{
	Q_OBJECT

public:
	explicit HotKeyWorker(QObject *parent = 0);
	~HotKeyWorker();

	void setHotKey(uint modifiers, int keyCode);

public Q_SLOTS:
	void listen();

Q_SIGNALS:
	void hotKeyTriggered();

private:
	QMutex m_mutex;
	QMutex m_hotKeyChange;
	Display* m_pDisplay;
	Window m_grabWindow;
	uint m_modifiers;
	int m_keyCode;
	bool m_handleNextEvent;
};

#endif // HOTKEYWORKER_H
