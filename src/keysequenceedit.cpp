#include "keysequenceedit.h"

#include <QKeyEvent>

KeySequenceEdit::KeySequenceEdit(QWidget *parent) : QKeySequenceEdit(parent)
{
}

KeySequenceEdit::~KeySequenceEdit()
{
}

void KeySequenceEdit::keyPressEvent(QKeyEvent *pEvent)
{
	QKeySequence seq(QKeySequence::fromString(keySequence().toString().split(", ").first()));
	setKeySequence(seq);

	QKeySequenceEdit::keyPressEvent(pEvent);
}

