#ifndef GNULINUXHOTKEYBINDER_H
#define GNULINUXHOTKEYBINDER_H

#include <QObject>
#include <QString>
#include <QKeySequence>

#include "gnulinuxhotkeyworker.h"

class GNULinuxHotKeyWorker;
class QThread;

class GNULinuxHotKeyBinder : public QObject
{
	Q_OBJECT

public:
	GNULinuxHotKeyBinder(QObject *parent = 0);
	~GNULinuxHotKeyBinder();

	QKeySequence keySequence() const;

public Q_SLOTS:
	void setHotKey(uint modifiers, uint key);

private:
	QKeySequence m_keySequence;

	GNULinuxHotKeyWorker* m_pHotKeyWorker;
	QThread* m_pHotKeyThread;
};

#endif // GNULINUXHOTKEYBINDER_H
