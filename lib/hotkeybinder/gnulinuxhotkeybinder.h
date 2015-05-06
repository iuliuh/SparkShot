#ifndef GNULINUXHOTKEYBINDER_H
#define GNULINUXHOTKEYBINDER_H

#include <QObject>
#include <QString>
#include <QKeySequence>

#include "gnulinuxhotkeyworker.h"

class HotKeyWorker;
class QThread;

class GNULinuxHotKeyBinder : public QObject
{
	Q_OBJECT

public:
	GNULinuxHotKeyBinder(QObject *parent = 0);
	~GNULinuxHotKeyBinder();

	QKeySequence keySequence() const;

public Q_SLOTS:
	void setKeySequence(const QString &key);

private:
	QKeySequence m_keySequence;

	HotKeyWorker* m_pHotKeyWorker;
	QThread* m_pHotKeyThread;
};

#endif // GNULINUXHOTKEYBINDER_H
