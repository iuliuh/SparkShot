#ifndef HOTKEYBINDER_H
#define HOTKEYBINDER_H

#include <QObject>

#include "hotkeybinder_global.h"

#ifdef Q_OS_LINUX
#include "gnulinuxhotkeybinder.h"
#endif

#ifdef Q_OS_WIN
#include "winhotkeybinder.h"
#endif

class HOTKEYBINDERSHARED_EXPORT HotKeyBinder : public QObject
{
	Q_OBJECT

public:
	explicit HotKeyBinder(QObject *parent = 0);
	~HotKeyBinder();

	void setHotKey(const QString& keySequence);

Q_SIGNALS:
	void hotKeyTriggered();

private:
#ifdef Q_OS_WIN
	WinHotKeyBinder* m_pWinHotKeyBinder;
#endif
};

#endif // HOTKEYBINDER_H
