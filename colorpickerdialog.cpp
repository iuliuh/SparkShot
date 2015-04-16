#include "colorpickerdialog.h"
#include "ui_colorpickerdialog.h"

#include <QPainter>

ColorPickerDialog::ColorPickerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorPickerDialog)
{
	ui->setupUi(this);

	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | Qt::BypassWindowManagerHint);
	setAttribute(Qt::WA_TranslucentBackground);

	resize(100, 100);
}

ColorPickerDialog::~ColorPickerDialog()
{
	delete ui;
}

void ColorPickerDialog::paintEvent(QPaintEvent* e)
{
	Q_UNUSED(e)

	// Draw the popup here
	// You can always pick an image and use drawPixmap to
	// draw it in order to make things simpler

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

	// Draw the popup pointer
	const QPointF points[3] = {
		QPoint(roundedRectDimensions.x() + roundedRectDimensions.width() / 2 - x / 2, roundedRectDimensions.y()),
		QPoint(roundedRectDimensions.x() + roundedRectDimensions.width() / 2 + x / 2, roundedRectDimensions.y()),
		QPoint(roundedRectDimensions.x() + roundedRectDimensions.width() / 2, roundedRectDimensions.y() - x / 2)
	};

	painter.drawPolygon(points, 3);

	QWidget::paintEvent(e);
}
