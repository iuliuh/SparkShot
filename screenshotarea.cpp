#include "screenshotarea.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include <QKeyEvent>
#include <QPainter>

ScreenshotArea::ScreenshotArea(QWidget *parent) : QWidget(parent)
{
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

	m_pToolBar = new ToolBar(ToolBar::Horizontal, this);
	m_pToolBar->move(100, 100);
	m_pToolBar->show();
	m_darkOverlayColor = QColor(0, 0, 0, 155);
	m_rubberBandColor = Qt::cyan;
	m_rubberBandWidth = 2;
	m_rubberBandPointRadius = 3;

	m_leftButtonPressed = false;
	m_selectionStarted = false;

	connect(m_pToolBar, &ToolBar::discardButtonPressed,
			this, &QWidget::close);
	connect(m_pToolBar, &ToolBar::settingsButtonPressed,
			this, &ScreenshotArea::onSettingsButtonPressed);
	connect(m_pToolBar, &ToolBar::uploadButtonPressed,
			this, &ScreenshotArea::onUploadButtonPressed);
	connect(m_pToolBar, &ToolBar::saveButtonPressed,
			this, &ScreenshotArea::onSaveButtonPressed);
	connect(m_pToolBar, &ToolBar::toolButtonChanged,
			this, &ScreenshotArea::onToolButtonChanged);

	setGeometry(QGuiApplication::primaryScreen()->geometry());
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

	QScreen *pScreen = QGuiApplication::primaryScreen();
	if (pScreen)
	{
		m_originalCapture = pScreen->grabWindow(0);
	}

	show();
	activateWindow();
}

void ScreenshotArea::onSettingsButtonPressed()
{
	qDebug() << Q_FUNC_INFO;
}

void ScreenshotArea::onUploadButtonPressed()
{
	qDebug() << Q_FUNC_INFO;
}

void ScreenshotArea::onSaveButtonPressed()
{
	qDebug() << Q_FUNC_INFO;
}

void ScreenshotArea::onToolButtonChanged(const ToolBar::Tool& tool)
{
	qDebug() << tool;
}

void ScreenshotArea::mouseMoveEvent(QMouseEvent *e)
{
	if(m_leftButtonPressed)
	{
		m_currentPressPoint = e->pos();

		m_pToolBar->move(m_currentPressPoint += QPoint(10, 10));

		repaint();
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
		m_pToolBar->hide();
		repaint();
	}
}

void ScreenshotArea::mouseReleaseEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = false;
		m_pToolBar->show();
		repaint();
	}
}

void ScreenshotArea::paintEvent(QPaintEvent *pEvent)
{
	QPainter painter(this);

	// Draw dark overlay
	QPixmap drawnCapture = m_originalCapture;

	painter.drawPixmap(0, 0, drawnCapture.width(), drawnCapture.height(), drawnCapture);

	QBrush darkOverlayBrush(m_darkOverlayColor);

	painter.setBrush(darkOverlayBrush);
	painter.drawRect(pEvent->rect());

	// Draw original section
	if(m_selectionStarted)
	{
		QRect pixmapRect(m_initialPressPoint.x() + m_rubberBandWidth,
						 m_initialPressPoint.y() + m_rubberBandWidth,
						 m_currentPressPoint.x() - m_initialPressPoint.x(),
						 m_currentPressPoint.y() - m_initialPressPoint.y());

		painter.drawPixmap(pixmapRect.normalized(), m_originalCapture, pixmapRect.normalized());
	}

	// Draw rubber band
	QRect rubberBandRect(m_initialPressPoint.x() + m_rubberBandWidth,
						 m_initialPressPoint.y() + m_rubberBandWidth,
						 m_currentPressPoint.x() - m_initialPressPoint.x(),
						 m_currentPressPoint.y() - m_initialPressPoint.y());
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

	update();
}
