#include "toolbardialog.h"

#include <QPainter>

ToolBarDialog::ToolBarDialog(QWidget *pParent) :
    QDialog(pParent)
{
	setWindowFlags(Qt::Tool |
	               Qt::FramelessWindowHint |
	               Qt::NoDropShadowWindowHint |
	               Qt::BypassWindowManagerHint);

	setAttribute(Qt::WA_TranslucentBackground);

	m_arrowLocation = Top;
}

ToolBarDialog::~ToolBarDialog()
{
}

void ToolBarDialog::setArrowLocation(ToolBarDialog::ArrowLocation arrowLocation)
{
	m_arrowLocation = arrowLocation;

	update();
}

ToolBarDialog::ArrowLocation ToolBarDialog::arrowLocation() const
{
	return m_arrowLocation;
}

void ToolBarDialog::paintEvent(QPaintEvent* pPaintEvent)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// Prepare the popup dimensions
	QRect roundedRectDimensions;
	roundedRectDimensions.setX(rect().x() + 10);
	roundedRectDimensions.setY(rect().y() + 10);
	roundedRectDimensions.setWidth(rect().width() - 20);
	roundedRectDimensions.setHeight(rect().height() - 20);

	painter.setBrush(QBrush(Qt::lightGray));

	QPen pen;
	pen.setColor(Qt::gray);
	pen.setWidth(2);
	painter.setPen(pen);

	// Draw the popup body
	painter.drawRoundedRect(roundedRectDimensions, 3, 3);

	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(Qt::gray));

	int x = 10;

	QPointF p1, p2, p3;
	switch(m_arrowLocation)
	{
	case Top:
		p1.setX(roundedRectDimensions.x() + roundedRectDimensions.width() / 2 - x / 2);
		p1.setY(roundedRectDimensions.y());
		p2.setX(roundedRectDimensions.x() + roundedRectDimensions.width() / 2 + x / 2);
		p2.setY(roundedRectDimensions.y());
		p3.setX(roundedRectDimensions.x() + roundedRectDimensions.width() / 2);
		p3.setY(roundedRectDimensions.y() - x / 2);
		break;
	case Right:
		p1.setX(roundedRectDimensions.x() + roundedRectDimensions.width());
		p1.setY(roundedRectDimensions.y() + roundedRectDimensions.height() / 2 - x / 2);
		p2.setX(roundedRectDimensions.x() + roundedRectDimensions.width());
		p2.setY(roundedRectDimensions.y() + roundedRectDimensions.height() / 2 + x / 2);
		p3.setX(roundedRectDimensions.x() + roundedRectDimensions.width() + x / 2);
		p3.setY(roundedRectDimensions.y() + roundedRectDimensions.height() / 2);
		break;
	case Bottom:
		p1.setX(roundedRectDimensions.x() + roundedRectDimensions.width() / 2 - x / 2);
		p1.setY(roundedRectDimensions.y() + roundedRectDimensions.height());
		p2.setX(roundedRectDimensions.x() + roundedRectDimensions.width() / 2 + x / 2);
		p2.setY(roundedRectDimensions.y() + roundedRectDimensions.height());
		p3.setX(roundedRectDimensions.x() + roundedRectDimensions.width() / 2);
		p3.setY(roundedRectDimensions.y() + roundedRectDimensions.height() + x / 2);
		break;
	case Left:
		p1.setX(roundedRectDimensions.x());
		p1.setY(roundedRectDimensions.y() + roundedRectDimensions.height() / 2 - x / 2);
		p2.setX(roundedRectDimensions.x());
		p2.setY(roundedRectDimensions.y() + roundedRectDimensions.height() / 2 + x / 2);
		p3.setX(roundedRectDimensions.x() - x / 2);
		p3.setY(roundedRectDimensions.y() + roundedRectDimensions.height() / 2);
		break;
	default:
		break;
	}

	// Draw the popup pointer
	const QPointF points[3] = { p1, p2, p3 };

	painter.drawPolygon(points, 3);

	QWidget::paintEvent(pPaintEvent);
}
