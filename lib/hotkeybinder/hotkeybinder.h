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

	bool setHotKey(const QString& keySequence);

	QString toString();

Q_SIGNALS:
	void hotKeyTriggered();

private:
	QString m_keySequence;

#ifdef Q_OS_WIN
	WinHotKeyBinder* m_pWinHotKeyBinder;
#endif

#ifdef Q_OS_LINUX
	GNULinuxHotKeyBinder* m_pGNULinuxHotBinder;
#endif
};

#endif // HOTKEYBINDER_H
