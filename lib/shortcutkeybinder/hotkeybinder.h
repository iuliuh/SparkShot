#ifndef SHORTCUTKEYBINDER_H
#define SHORTCUTKEYBINDER_H

#include "shortcutkeybinder_global.h"

#include <QObject>
#include <QString>
#include <QKeySequence>

#include "hotkeyworker.h"

class HotKeyWorker;
class QThread;

class SHORTCUTKEYBINDERSHARED_EXPORT HotKeyBinder : public QObject
{

public:
	HotKeyBinder(QObject *parent = 0);
	~HotKeyBinder();

	QKeySequence keySequence() const;

public Q_SLOTS:
	void setKeySequence(const QString &key);

private:
	QKeySequence m_keySequence;
	HotKeyWorker* m_pHotKeyWorker;
	QThread* m_pHotKeyThread;
};

#endif // SHORTCUTKEYBINDER_H
