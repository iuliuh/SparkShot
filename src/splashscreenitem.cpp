#include "splashscreenitem.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

SplashScreenItem::SplashScreenItem(QWidget *parent) : QWidget(parent)
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
	Q_UNUSED(pPaintEvent)

	QPainter painter(this);
	painter.setOpacity(m_alpha);
	painter.drawPixmap(m_pixmap.rect(), m_pixmap, m_pixmap.rect());
}

void SplashScreenItem::enterEvent(QEvent *pEvent)
{
	Q_UNUSED(pEvent)

	if(m_url.isValid())
	{
		setCursor(Qt::PointingHandCursor);
	}
}

void SplashScreenItem::leaveEvent(QEvent *pEvent)
{
	Q_UNUSED(pEvent)

	setCursor(Qt::ArrowCursor);
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



