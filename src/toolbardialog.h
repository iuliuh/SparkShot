#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

//! \brief This is the base class for all dialogs that are
//!        ment to be launched directly from the toolbar.
class ToolBarDialog : public QDialog
{
	Q_OBJECT

public:
	//! \brief Dialog arrow location
	enum ArrowLocation
	{
		Top,    //!< Dialog arrow is positioned on top of the dialog.
		Right,  //!< Dialog arrow is positioned on the right side of the dialog.
		Bottom, //!< Dialog arrow is positioned on the bottom of the dialog.
		Left    //!< Dialog arrow is positioned on the left side of the dialog.
	};

	//! \brief Constructs a ToolBarDialog object.
	//! \param pParent Parent object.
	explicit ToolBarDialog(QWidget *pParent = 0);

	//! \brief Destroys a ToolBarDialog object.
	~ToolBarDialog();

	//! \brief Sets the dialog arrow location.
	//! \param arrowLocation The location of the arrow.
	//! \see ArrowLocation
	void setArrowLocation(ArrowLocation arrowLocation);

	//! \brief Provides the dialog arrow location.
	//! \return The location of the arrow.
	//! \see ArrowLocation
	ArrowLocation arrowLocation() const;

protected:
	//! \see QWidget::paintEvent
	void paintEvent(QPaintEvent *pPaintEvent);

private:
	// Location of the arrow on the dialog
	ArrowLocation m_arrowLocation;
};

#endif // DIALOG_H
