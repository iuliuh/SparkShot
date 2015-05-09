#ifndef KEYSEQUENCEEDIT_H
#define KEYSEQUENCEEDIT_H

// Qt includes
#include <QKeySequenceEdit>

// Forward declarations
class QKeyEvent;

//! \brief Class derived from QKeySequenceEdit which doesn't
//!        support more than one key sequence as opposed to
//!        QKeySequenceEdit which supports 4.
class KeySequenceEdit : public QKeySequenceEdit
{
	Q_OBJECT

public:
	//! \brief Constructs a GNULinuxHotKeyBinder object.
	//! \brief pParent Parent object.
	explicit KeySequenceEdit(QWidget *pParent = 0);

	//! \brief Destroys a GNULinuxHotKeyBinder object.
	~KeySequenceEdit();

protected:
	//! \see QWidget::keyPressEvent
	void keyPressEvent(QKeyEvent *pEvent);
};

#endif // KEYSEQUENCEEDIT_H
