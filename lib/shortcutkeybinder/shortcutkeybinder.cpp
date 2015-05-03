#include "shortcutkeybinder.h"

ShortcutKeyBinder::ShortcutKeyBinder(QObject *parent) : QObject(parent)
{
}

ShortcutKeyBinder::~ShortcutKeyBinder()
{
}

QKeySequence ShortcutKeyBinder::keySequence() const
{
	return m_keySequence;
}

void ShortcutKeyBinder::setKeySequence(const QKeySequence &keySequence)
{
	m_keySequence = keySequence;
}

