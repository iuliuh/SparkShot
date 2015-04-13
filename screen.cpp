#include "screen.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include <QKeyEvent>
#include <QPainter>

ScreenshotArea::ScreenshotArea(QWidget *parent) : QWidget(parent)
{
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);

	m_darkOverlayColor = QColor(0, 0, 0, 128);
	m_rubberBandColor = Qt::cyan;
	m_rubberBandWidth = 2;
	m_rubberBandPointRadius = 3;

	m_pDrawingBoard = new QLabel;
	m_leftButtonPressed = false;
	m_selectionStarted = false;

	m_pLayout = new QGridLayout;
	m_pLayout->addWidget(m_pDrawingBoard);
	m_pLayout->setSpacing(0);
	m_pLayout->setMargin(0);

	setLayout(m_pLayout);
}

ScreenshotArea::~ScreenshotArea()
{

}

void ScreenshotArea::keyPressEvent(QKeyEvent *pEvent)
{
	bool pressedAltF4 = pEvent->key() == Qt::Key_F4 &&
	                    pEvent->modifiers() == Qt::AltModifier;

	bool pressedEsc = pEvent->key() == Qt::Key_Escape;

	// If user hits Esc or Alt+F4 the application closes
	if(pressedAltF4 || pressedEsc)
	{
		close();
	}

	QWidget::keyPressEvent(pEvent);
}

void ScreenshotArea::shoot()
{
	m_originalCapture = QPixmap();

	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen)
	{
		m_originalCapture = screen->grabWindow(0);
	}

	show();
	activateWindow();
}

void ScreenshotArea::mouseMoveEvent(QMouseEvent *e)
{
	if(m_leftButtonPressed)
	{
		m_currentPressPoint = e->pos();
	}
}

void ScreenshotArea::mousePressEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = true;
		m_selectionStarted = true;
		m_initialPressPoint = e->pos();
		m_currentPressPoint = e->pos();
	}
}

void ScreenshotArea::mouseReleaseEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = false;
	}
}

void ScreenshotArea::paintEvent(QPaintEvent *pEvent)
{
	QPixmap drawnCapture = m_originalCapture;

	QPainter painter(&drawnCapture);

	// Draw dark overlay
	QBrush darkOverlayBrush(m_darkOverlayColor);

	painter.setBrush(darkOverlayBrush);
	painter.drawRect(pEvent->rect());

	// Draw original section
	if(m_selectionStarted)
	{
		QRect pixmapRect(m_initialPressPoint.x() + m_rubberBandWidth - 1,
		                 m_initialPressPoint.y() + m_rubberBandWidth - 1,
		                 m_currentPressPoint.x() - m_initialPressPoint.x() - m_rubberBandWidth,
		                 m_currentPressPoint.y() - m_initialPressPoint.y() - m_rubberBandWidth);

		painter.drawPixmap(pixmapRect.normalized(), m_originalCapture, pixmapRect.normalized());
	}

	// Draw rubber band
	QRect rubberBandRect(m_initialPressPoint.x() + m_rubberBandWidth - 1,
	                     m_initialPressPoint.y() + m_rubberBandWidth - 1,
	                     m_currentPressPoint.x() - m_initialPressPoint.x() - m_rubberBandWidth,
	                     m_currentPressPoint.y() - m_initialPressPoint.y() - m_rubberBandWidth);
	QPen pen(m_rubberBandColor);
	pen.setWidth(m_rubberBandWidth);

	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(rubberBandRect.normalized());

	painter.setBrush(m_rubberBandColor);

	QPoint topMiddle(rubberBandRect.topLeft().x() + rubberBandRect.width() / 2, rubberBandRect.y());
	QPoint rightMiddle(rubberBandRect.topLeft().x() + rubberBandRect.width(), rubberBandRect.y() + rubberBandRect.height() / 2);
	QPoint bottomMiddle(rubberBandRect.topLeft().x() + rubberBandRect.width() / 2, rubberBandRect.y() + rubberBandRect.height());
	QPoint leftMiddle(rubberBandRect.topLeft().x(), rubberBandRect.y() + rubberBandRect.height() / 2);

	painter.drawEllipse(rubberBandRect.topLeft(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter.drawEllipse(rubberBandRect.topRight(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter.drawEllipse(rubberBandRect.bottomLeft(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter.drawEllipse(rubberBandRect.bottomRight(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter.drawEllipse(topMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter.drawEllipse(rightMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter.drawEllipse(bottomMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter.drawEllipse(leftMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);

	m_pDrawingBoard->setPixmap(drawnCapture);
}







