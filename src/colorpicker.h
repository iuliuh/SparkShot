#ifndef COLORPICKER_H
#define COLORPICKER_H

// Local includes
#include "toolbardialog.h"

// Qt includes
#include <qmath.h>

//! \brief Class representing a widget that allows the selection of a HSV color.
//!        It has an outer wheel to select the Hue and an intenal square to
//!        select Saturation and Lightness.
class ColorPicker : public ToolBarDialog
{
	Q_OBJECT

	//! \brief The color picker color.
	Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

	//! \brief The color picker hue value.
	Q_PROPERTY(qreal hue READ hue WRITE setHue)

	//! \brief The color picker saturation value.
	Q_PROPERTY(qreal saturation READ saturation WRITE setSaturation)

	//! \brief The color picker value.
	Q_PROPERTY(qreal value READ value WRITE setValue)

	//! \brief The color picker wheel width.
	Q_PROPERTY(uint wheelWidth READ wheelWidth WRITE setWheelWidth)

public:
	//! \brief Constructs a ColorPicker object.
	//! \param pParent Parent object.
	explicit ColorPicker(QWidget* pParent = 0);

	//! \brief Provides current color.
	//! \return Current color.
	QColor color() const;

	//! \brief Provides the size of the widget.
	//! \return Current widget size.
	QSize sizeHint() const;

	//! \brief Provides the current hue in the range [0-1].
	//! \return Hue value.
	qreal hue() const;

	//! \brief Provides the current saturation in the range [0-1].
	//! \return Saturation value.
	qreal saturation() const;

	//! \brief Provides the current value in the range [0-1].
	//! \return The current value.
	qreal value() const;

	//! \brief Get the width in pixels of the outer wheel.
	//! \return Wheel width.
	uint wheelWidth() const;

	//! \brief Set the width in pixels of the outer wheel.
	//! \param width The width of the wheel.
	void setWheelWidth(uint width);

public Q_SLOTS:
	//! \brief Set current color.
	//! \param c The color to set.
	void setColor(QColor c);

	//! \brief Sets the hue value.
	//! \param hue Hue value.
	void setHue(qreal hue);

	//! \brief Sets the saturation value.
	//! \param saturation Saturation value.
	void setSaturation(qreal saturation);

	//! \brief Sets the value.
	//! \param value Value.
	void setValue(qreal value);

Q_SIGNALS:
	//! \brief Signal emitted when the color changes.
	//! \param color The new color.
	void colorChanged(QColor color);

	//! \brief Signal emitted when a new color gets selected.
	//! \param color The new selected color.
	void colorSelected(QColor color);

protected:
	//! \see QWidget::paintEvent
	void paintEvent(QPaintEvent* pPaintEvent);

	//! \see QWidget::mouseMoveEvent
	void mouseMoveEvent(QMouseEvent* pMouseEvent);

	//! \see QWidget::mousePressEvent
	void mousePressEvent(QMouseEvent* pMouseEvent);

	//! \see QWidget::mouseReleaseEvent
	void mouseReleaseEvent(QMouseEvent* pMouseEvent);

	//! \see QWidget::resizeEvent
	void resizeEvent(QResizeEvent* pResizeEvent);

private:
	//! \brief Calculates outer wheel radius from widget center.
	//! \return The outer radius.
	qreal outerRadius() const;

	//! \brief Calculate inner wheel radius from idget center.
	//! \return The inner radius.
	qreal innerRadius() const;

	//! \brief Provides the line from center to given point.
	//! \return The line from center to given point.
	QLineF lineToPoint(QPoint p) const;

	//! \brief Calculate the edge length of the inner square.
	//! \return The edge length of the inner square
	qreal squareSize() const;

	//! \brief Updates the inner square that displays the saturation-value selector
	void renderRectangle();

	//! \brief Updates the outer ring that displays the hue selector
	void renderRing();

private:
	//! \brief Mouse states
	enum MouseStatus
	{
		Nothing,    //!< Mouse is released.
		DragCircle, //!< Mouse is dragging the circle.
		DragSquare  //!< Mouse is dragging the square.
	};

private:
	// Color values
	qreal m_hue;
	qreal m_saturation;
	qreal m_value;

	// Mouse states
	MouseStatus m_mouseStatus;

	// Wheel width
	uint m_wheelWidth;

	// Drawing assets
	QPixmap m_hueRing;
	QImage m_satValSquare;

	// Widget padding
	int m_padding;
};

#endif // COLORPICKER_H
