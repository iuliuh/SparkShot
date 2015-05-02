#ifndef KEYSEQUENCEEDIT_H
#define KEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>

class QKeyEvent;

class KeySequenceEdit : public QKeySequenceEdit
{
	Q_OBJECT

public:
	explicit KeySequenceEdit(QWidget *parent = 0);
	~KeySequenceEdit();

protected:
	void keyPressEvent(QKeyEvent *pEvent);
};

#endif // KEYSEQUENCEEDIT_H
