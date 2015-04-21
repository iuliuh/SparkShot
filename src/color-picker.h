#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include "dialog.h"
#include <qmath.h>
#include <QGridLayout>
#include <QLabel>

/**
 * \brief Display an analog widget that allows the selection of a HSV color
 *
 * It has an outer wheel to select the Hue and an intenal square to select
 * Saturation and Lightness.
 */
class ColorPicker : public Dialog
{
	Q_OBJECT

	Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE true STORED false )
	Q_PROPERTY(qreal hue READ hue WRITE setHue DESIGNABLE false )
	Q_PROPERTY(qreal saturation READ saturation WRITE setSaturation DESIGNABLE false )
	Q_PROPERTY(qreal value READ value WRITE setValue DESIGNABLE false )
	Q_PROPERTY(unsigned wheelWidth READ wheelWidth WRITE setWheelWidth DESIGNABLE true )

public:
	explicit ColorPicker(QWidget *parent = 0);

	// Get current color
	QColor color() const;

	QSize sizeHint () const;

	// Get current hue in the range [0-1]
	qreal hue() const;

	// Get current saturation in the range [0-1]
	qreal saturation() const;

	// Get current value in the range [0-1]
	qreal value() const;

	// Get the width in pixels of the outer wheel
	unsigned wheelWidth() const;

	// Set the width in pixels of the outer wheel
	void setWheelWidth(unsigned w);

public Q_SLOTS:
	// Set current color
	void setColor(QColor c);

	// Params
	void setHue(qreal hue);
	void setSaturation(qreal saturation);
	void setValue(qreal value);

Q_SIGNALS:
	void colorChanged(QColor);
	void colorSelected(QColor);

protected:
	void paintEvent(QPaintEvent *e);
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void resizeEvent(QResizeEvent *event);

private:
	// Calculate outer wheel radius from idget center
	qreal outerRadius() const;
	// Calculate inner wheel radius from idget center
	qreal innerRadius() const;
	// return line from center to given point
	QLineF lineToPoint(QPoint p) const;
	// Calculate the edge length of the inner square
	qreal squareSize() const;
	// Updates the inner square that displays the saturation-value selector
	void renderRectangle();
	// Updates the outer ring that displays the hue selector
	void renderRing();

private:
	qreal m_hue;
	qreal m_saturation;
	qreal m_value;
	unsigned m_wheelWidth;

	enum MouseStatus
	{
		Nothing,
		DragCircle,
		DragSquare
	} m_mouseStatus;

	QPixmap m_hueRing;
	QImage m_satValSquare;

	int m_padding;
};

#endif // COLORPICKER_H
