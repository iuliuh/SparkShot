#ifndef SHORTCUTKEYBINDER_H
#define SHORTCUTKEYBINDER_H

#include "shortcutkeybinder_global.h"

#include <QObject>
#include <QString>
#include <QKeySequence>

class SHORTCUTKEYBINDERSHARED_EXPORT ShortcutKeyBinder : public QObject
{

public:
	ShortcutKeyBinder(QObject *parent = 0);
	~ShortcutKeyBinder();

	QKeySequence keySequence() const;
	void setKeySequence(const QKeySequence &key);

private:
	QKeySequence m_keySequence;
};

#endif // SHORTCUTKEYBINDER_H
