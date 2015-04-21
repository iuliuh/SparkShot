#include "color-picker.h"

#include <QMouseEvent>
#include <QPainter>
#include <QLineF>
#include <QColor>

ColorPicker::ColorPicker(QWidget *parent) :
	Dialog(parent),
	m_hue(0),
	m_saturation(0),
	m_value(0),
	m_wheelWidth(10),
	m_mouseStatus(Nothing),
    m_padding(30)
{
	setFixedSize(100, 100);
}

qreal ColorPicker::outerRadius() const
{
	return qMin(geometry().width() - m_padding, geometry().height() - m_padding) / 2;
}

qreal ColorPicker::innerRadius() const
{
	return outerRadius()-m_wheelWidth;
}

QLineF ColorPicker::lineToPoint(QPoint p) const
{
	return QLineF (geometry().width() / 2,
	               geometry().height() / 2,
	               p.x(),
	               p.y());
}

qreal ColorPicker::squareSize() const
{
	return innerRadius()*qSqrt(2);
}

QColor ColorPicker::color() const
{
	return QColor::fromHsvF(m_hue, m_saturation, m_value);
}

QSize ColorPicker::sizeHint() const
{
	return QSize(m_wheelWidth * 5, m_wheelWidth * 5);
}

qreal ColorPicker::hue() const
{
	return m_hue;
}

qreal ColorPicker::saturation() const
{
	return m_saturation;
}

qreal ColorPicker::value() const
{
	return m_value;
}

unsigned ColorPicker::wheelWidth() const
{
	return m_wheelWidth;
}

void ColorPicker::setWheelWidth(unsigned w)
{
	m_wheelWidth = w;
	renderRectangle();
	update();
}

void ColorPicker::paintEvent(QPaintEvent * e)
{
	Dialog::paintEvent(e);

	double selectorW = 4;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.translate(geometry().width() / 2,
	                  geometry().height() / 2);

	// Hue wheel
	if ( m_hueRing.isNull() )
	{
		renderRing();
	}

	painter.drawPixmap(int(-outerRadius()), int(-outerRadius()), m_hueRing);

	// Hue selector
	painter.setPen(QPen(Qt::white, 2));
	painter.setBrush(Qt::NoBrush);
	QLineF ray(0, 0, outerRadius(), 0);
	ray.setAngle(m_hue * 360);
	QPointF h1 = ray.p2();
	ray.setLength(innerRadius());
	QPointF h2 = ray.p2();
	painter.drawLine(h1, h2);

	// Lum-sat square
	if (m_satValSquare.isNull())
	{
		renderRectangle();
	}

	painter.rotate(-m_hue * 360 - 45);
	ray.setLength(innerRadius());
	ray.setAngle(135);
	painter.drawImage(ray.p2(),m_satValSquare);

	// Lum-sat selector
	painter.setPen(QPen(Qt::white, 2));
	painter.setBrush(Qt::NoBrush);
	double max_dist = squareSize();
	painter.drawEllipse(QPointF(m_saturation * max_dist - (max_dist / 2),
	                            m_value * max_dist - (max_dist / 2)),
	                    selectorW,
	                    selectorW);
}

void ColorPicker::mouseMoveEvent(QMouseEvent *ev)
{
	if (m_mouseStatus == DragCircle)
	{
		m_hue = lineToPoint(ev->pos()).angle() / 360.0;
		renderRectangle();

		emit colorSelected(color());
		emit colorChanged(color());
		update();
	}
	else if (m_mouseStatus == DragSquare)
	{
		QLineF glob_mouse_ln = lineToPoint(ev->pos());
		QLineF center_mouse_ln(QPointF(0, 0),
		                       glob_mouse_ln.p2() - glob_mouse_ln.p1());
		center_mouse_ln.setAngle(center_mouse_ln.angle() - m_hue * 360 - 45);

		m_saturation = center_mouse_ln.x2() / squareSize() + 0.5;

		m_value = center_mouse_ln.y2() / squareSize() + 0.5;

		if (m_saturation > 1)
		{
			m_saturation = 1;
		}
		else if (m_saturation < 0)
		{
			m_saturation = 0;
		}

		if (m_value > 1)
		{
			m_value = 1;
		}
		else if (m_value < 0)
		{
			m_value = 0;
		}

		emit colorSelected(color());
		emit colorChanged(color());
		update();
	}
}

void ColorPicker::mousePressEvent(QMouseEvent *ev)
{
	if (ev->buttons() & Qt::LeftButton)
	{
		QLineF ray = lineToPoint(ev->pos());
		if (ray.length() <= innerRadius())
		{
			m_mouseStatus = DragSquare;
		}
		else if (ray.length() <= outerRadius())
		{
			m_mouseStatus = DragCircle;
		}
	}
}

void ColorPicker::mouseReleaseEvent(QMouseEvent *ev)
{
	mouseMoveEvent(ev);
	m_mouseStatus = Nothing;
}

void ColorPicker::resizeEvent(QResizeEvent *event)
{
	renderRing();
	renderRectangle();
	QWidget::resizeEvent(event);
}

void ColorPicker::renderRectangle()
{
	int size = int(squareSize());
	m_satValSquare = QImage(size, size, QImage::Format_RGB32);

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			QColor pixelValue = QColor::fromHsvF(m_hue, double(i) / size, double(j) / size);

			if(!isEnabled())
			{
				const int grayValue = (pixelValue.red() + pixelValue.green() + pixelValue.blue()) / 3;
				pixelValue.setRgb(grayValue, grayValue, grayValue);
				pixelValue.setAlpha(127);
			}

			m_satValSquare.setPixel( i, j, pixelValue.rgb());
		}
	}
}

void ColorPicker::renderRing()
{
	m_hueRing = QPixmap(int(outerRadius()) * 2, int(outerRadius()) * 2);
	m_hueRing.fill(Qt::transparent);
	QPainter painter(&m_hueRing);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setCompositionMode(QPainter::CompositionMode_Source);

	const int hueStops = 24;
	QConicalGradient gradientHue(0, 0, 0);

	if (gradientHue.stops().size() < hueStops)
	{
		for (double pos = 0; pos < 1.0; pos+=1.0 / (hueStops - 1))
		{
			QColor gradientColor = QColor::fromHsvF(pos, 1, 1);

			if(!isEnabled())
			{
				const int grayColor = (gradientColor.red() + gradientColor.green() + gradientColor.blue()) / 3;
				gradientColor.setRgb(grayColor, grayColor, grayColor);
				gradientColor.setAlpha(127);
			}

			gradientHue.setColorAt(pos, gradientColor);
		}
		gradientHue.setColorAt(1, QColor::fromHsvF(0, 1, 1));
	}

	painter.translate(outerRadius(),outerRadius());

	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(gradientHue));
	painter.drawEllipse(QPointF(0, 0), outerRadius(), outerRadius());

	painter.setBrush(Qt::transparent);//palette().background());
	painter.drawEllipse(QPointF(0, 0), innerRadius(), innerRadius());
}

void ColorPicker::setColor(QColor color)
{
	qreal oldH = m_hue;
	m_hue = color.hueF();

	if (m_hue < 0)
	{
		m_hue = 0;
	}

	m_saturation = color.saturationF();
	m_value = color.valueF();

	if (!qFuzzyCompare(oldH + 1, m_hue + 1))
	{
		renderRectangle();
	}

	update();
}

void ColorPicker::setHue(qreal hue)
{
	m_hue = qMax(0.0, qMin(1.0, hue));
	renderRectangle();
	update();
}

void ColorPicker::setSaturation(qreal saturation)
{
	m_saturation = qMax(0.0, qMin(1.0, saturation));
	update();
}

void ColorPicker::setValue(qreal value)
{
	m_value = qMax(0.0, qMin(1.0, value));
	update();
}

