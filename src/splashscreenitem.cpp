#include "splashscreenitem.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

SplashScreenItem::SplashScreenItem(QWidget *pParent) : QWidget(pParent)
{
	m_alpha = 0.0;

	setMouseTracking(true);
}

SplashScreenItem::~SplashScreenItem()
{
}

QPixmap SplashScreenItem::pixmap() const
{
	return m_pixmap;
}

void SplashScreenItem::setPixmap(const QPixmap &pixmap)
{
	m_pixmap = pixmap;

	setFixedSize(pixmap.size());
}

void SplashScreenItem::paintEvent(QPaintEvent *pPaintEvent)
{
	QPainter painter(this);
	painter.setOpacity(m_alpha);
	painter.drawPixmap(m_pixmap.rect(), m_pixmap, m_pixmap.rect());

	QWidget::paintEvent(pPaintEvent);
}

void SplashScreenItem::enterEvent(QEvent *pEvent)
{
	if(m_url.isValid())
	{
		setCursor(Qt::PointingHandCursor);
	}

	QWidget::enterEvent(pEvent);
}

void SplashScreenItem::leaveEvent(QEvent *pEvent)
{
	setCursor(Qt::ArrowCursor);

	SplashScreenItem::leaveEvent(pEvent);
}

QUrl SplashScreenItem::url() const
{
	return m_url;
}

void SplashScreenItem::setUrl(const QUrl &url)
{
	m_url = url;
}

qreal SplashScreenItem::alpha() const
{
	return m_alpha;
}

void SplashScreenItem::setAlpha(const qreal &alpha)
{
	m_alpha = alpha;

	repaint();
}



