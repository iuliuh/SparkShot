#ifndef SETTINGS_H
#define SETTINGS_H

// Local includes
#include "toolbardialog.h"
#include "preferences.h"

// Forward declarations
class QGridLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QHBoxLayout;
class QSpacerItem;
class ColorPicker;

//! \brief Class representing the quock settings dialog. This makes
//!        it easier to change settings like pen size, text font size,
//!        rubber band width or overlay color directly from the toolbar.
class QuickSettingsDialog : public ToolBarDialog
{
	Q_OBJECT

public:
	//! \brief Constructs a QuickSettingsDialog object.
	//! \brief pParent Parent object.
	explicit QuickSettingsDialog(QWidget *parent = 0);

	//! \brief Destroys a QuickSettingsDialog object.
	~QuickSettingsDialog();

	//! \brief Sets the overlay color.
	//! \param overlayColor The color of the snapshot overlay.
	void setOverlayColor(const QColor &overlayColor);

	//! \brief Provides the snapshot overlay color.
	//! \return The color of the snapshot overlay.
	QColor overlayColor() const;

	//! \brief Sets the rubber band color.
	//! \param rubberBandColor Color of the rubber band.
	void setRubberBandColor(const QColor &rubberBandColor);

	//! \brief Provides the color of the rubber band.
	//! \return Current rubber band color.
	QColor rubberBandColor() const;

	//! \brief Sets the rubber band width.
	//! \param rubberBandWidth The width of the rubber band.
	void setRubberBandWidth(int rubberBandWidth);

	//! \brief Provides the width of the rubber band.
	//! \return The current rubber band width.
	int rubberBandWidth() const;

	//! \brief Sets the pen width.
	//! \param penWidt The width of the pen.
	void setPenWidth(int penWidth);

	//! \brief Provides the width of the pen.
	//! \return The current pen width.
	int penWidth() const;

	//! \brief Sets the radius of the rubber band dots.
	//! \param dotsRadius The radius of the dots.
	void setDotsRadius(int dotsRadius);

	//! \brief Provides the rubber band dots radius.
	//! \return The radius of the rubber band dots.
	int dotsRadius() const;

	//! \brief Sets the font size.
	//! \param fontSize The size of the text font.
	void setFontSize(int fontSize);

	//! \brief Provides the font text font size.
	//! \return Returns the size of the font.
	int fontSize() const;

Q_SIGNALS:
	//! \brief Signal emitted when the overlay color changes.
	//! \param color The new overlay color.
	void overlayColorChanged(QColor color);

	//! \brief Signal emitted when the rubber band color changes.
	//! \param color The new rubber band color.
	void rubberBandColorChanged(QColor color);

	//! \brief Signal emitted when the rubber band width changes.
	//! \param width The new width of the rubber band.
	void rubberBandWidthChanged(int width);

	//! \brief Signal emitted when the pen width changes.
	//! \param width The new width of the pen.
	void penWidthChanged(int width);

	//! \brief Signal emitted when the radius of the rubber band dots change.
	//! \param radius The new radius of the rubber band dots.
	void dotsRadiusChanged(int radius);

	//! \brief Signal emitted when the  text font size changes.
	//! \param size The new size of the text font.
	void fontSizeChanged(int size);

protected:
	//! \see QWidget::moveEvent
	void moveEvent(QMoveEvent *pEvent);

	//! \see QWidget::keyPressEvent
	void keyPressEvent(QKeyEvent* pEvent);

private Q_SLOTS:
	//! \brief Slot called when the overlay color button is clicked.
	void onOverlayColorButtonClicked();

	//! \brief Slot called when the overlay color changes.
	//! \param newColor The new color of the snapshot overlay.
	void onOverlayColorChanged(QColor newColor);

	//! \brief Slot called when the rubber band color button is clicked.
	void onRubberBandColorButtonClicked();

	//! \brief Slot called when the rubber band color has changed.
	//! \param newColor The new color of the rubber band.
	void onRubberBandColorChanged(QColor newColor);

	//! \brief Slot called when the width of the rubber band has changed.
	//! \param width The new rubber band width.
	void onRubberBandWidthChanged(int width);

	//! \brief Slot called when the pen width has changed.
	//! \param width The new pen width.
	void onPenWidthChanged(int width);

	//! \brief Slot called when the text font size changes.
	//! \param size The new font size.
	void onFontSizeChanged(int size);

	//! \brief Slot called when the reset button has been clicked.
	void onResetButtonClicked();

private:
	// Layuout
	QGridLayout *m_pLayout;

	// Overlay color
	QLabel *m_pOverlayColorLabel;
	QPushButton *m_pOverlayColorButton;

	// Rubber band color
	QLabel *m_pRubberBandColorLabel;
	QPushButton *m_pRubberBandColorButton;

	// Pen width
	QLabel *m_pPenWidthLabel;
	QSpinBox *m_pPenWidthSpinBox;

	// Rubber band width
	QLabel *m_pRubberBandWidthLabel;
	QSpinBox *m_pRubberBandWidthSpinBox;

	// Font size
	QLabel *m_pFontSizeLabel;
	QSpinBox *m_pFontSizeSpinBox;

	// Rubber band dots radius
	int m_dotsRadius;

	// Buttons
	QHBoxLayout *m_pResetDefaultLayout;
	QPushButton *m_pResetDefaultsButton;

	// Spacers
	QSpacerItem *m_pResetDefaultsSpacer;
	QSpacerItem *m_pVerticalSpacer;

	// Color pickers
	ColorPicker *m_pOverlayColorPicker;
	ColorPicker *m_pRubberBandColorPicker;
};

#endif // SETTINGS_H
