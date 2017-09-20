#include "splashscreen.h"
#include "splashscreenitem.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMouseEvent>
#include <QStyleOption>
#include <QStyle>
#include <QPainter>
#include <QBrush>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

SplashScreen::SplashScreen(QWidget *pParent) :
    QWidget(pParent),
    m_splashScreenPixmap(":/images/splashScreen"),
    m_sidePanelPixmap(":/images/splashScreenSidePanel"),
    m_splasScreenBlured(":/images/splashScreenBlured"),
    m_sidePanelWidth(0)
{
	setWindowFlags(Qt::SplashScreen);
	setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_TranslucentBackground);

	setFixedSize(m_splashScreenPixmap.size());
	setWindowOpacity(0);

	m_pOpacityAnimation = new QPropertyAnimation(this, "windowOpacity");
	m_pOpacityAnimation->setDuration(500);
	m_pOpacityAnimation->setStartValue(0.0);
	m_pOpacityAnimation->setEndValue(1.0);
	m_pOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);

	m_pSidePanelWidthAnimation = new QVariantAnimation(this);
	m_pSidePanelWidthAnimation->setDuration(2000);
	m_pSidePanelWidthAnimation->setStartValue(0);
	m_pSidePanelWidthAnimation->setEndValue(200);
	m_pSidePanelWidthAnimation->setEasingCurve(QEasingCurve::OutCirc);

	m_pSidePanelWidthAnimation = new QVariantAnimation(this);
	m_pSidePanelWidthAnimation->setDuration(2000);
	m_pSidePanelWidthAnimation->setStartValue(0);
	m_pSidePanelWidthAnimation->setEndValue(200);
	m_pSidePanelWidthAnimation->setEasingCurve(QEasingCurve::OutCirc);

	connect(m_pOpacityAnimation, &QPropertyAnimation::finished,
	        this, &SplashScreen::onOpacityAnimationFinished);
	connect(m_pSidePanelWidthAnimation, &QVariantAnimation::valueChanged,
	        this, &SplashScreen::onSidePanelWidthValueChanged);
	connect(m_pSidePanelWidthAnimation, &QPropertyAnimation::finished,
	        this, &SplashScreen::onSidePanelWidthAnimationFinished);

	// Tool Item
	m_pSplashScreenToolItem = new SplashScreenItem(this);
	m_pSplashScreenToolItem->setUrl(QUrl("http://www.qt.io/"));
	m_pSplashScreenToolItem->move(450, 45);
	m_pSplashScreenToolItem->setPixmap(QPixmap(":/images/splashScreenSidePanelTool"));

	m_pSidePanelToolItemYAnimation = new QPropertyAnimation(m_pSplashScreenToolItem, "pos", this);
	m_pSidePanelToolItemYAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	m_pSidePanelToolItemYAnimation->setDuration(4000);
	m_pSidePanelToolItemYAnimation->setStartValue(QPoint(450, 25));
	m_pSidePanelToolItemYAnimation->setKeyValueAt(0.25, QPoint(450, 45));
	m_pSidePanelToolItemYAnimation->setKeyValueAt(0.75, QPoint(450, 45));
	m_pSidePanelToolItemYAnimation->setEndValue(QPoint(450, 65));

	m_pSidePanelToolItemOpacityAnimation = new QPropertyAnimation(m_pSplashScreenToolItem, "alpha", this);
	m_pSidePanelToolItemOpacityAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	m_pSidePanelToolItemOpacityAnimation->setDuration(4000);
	m_pSidePanelToolItemOpacityAnimation->setStartValue(0);
	m_pSidePanelToolItemOpacityAnimation->setKeyValueAt(0.25, 1.0);
	m_pSidePanelToolItemOpacityAnimation->setKeyValueAt(0.75, 1.0);
	m_pSidePanelToolItemOpacityAnimation->setEndValue(0);

	m_pSidePanelToolItemAnimationGroup = new QParallelAnimationGroup(this);
	m_pSidePanelToolItemAnimationGroup->addAnimation(m_pSidePanelToolItemYAnimation);
	m_pSidePanelToolItemAnimationGroup->addAnimation(m_pSidePanelToolItemOpacityAnimation);

	// Support Item
	m_pSplashScreenSupportItem = new SplashScreenItem(this);
	m_pSplashScreenSupportItem->setUrl(QUrl("http://devtelsoftware.com/"));
	m_pSplashScreenSupportItem->move(450, 45);
	m_pSplashScreenSupportItem->setPixmap(QPixmap(":/images/splashScreenSidePanelSupport"));

	m_pSidePanelSupportItemYAnimation = new QPropertyAnimation(m_pSplashScreenSupportItem, "pos", this);
	m_pSidePanelSupportItemYAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	m_pSidePanelSupportItemYAnimation->setDuration(4000);
	m_pSidePanelSupportItemYAnimation->setStartValue(QPoint(450, 25));
	m_pSidePanelSupportItemYAnimation->setKeyValueAt(0.25, QPoint(450, 45));
	m_pSidePanelSupportItemYAnimation->setKeyValueAt(0.75, QPoint(450, 45));
	m_pSidePanelSupportItemYAnimation->setEndValue(QPoint(450, 65));

	m_pSidePanelSupportItemOpacityAnimation = new QPropertyAnimation(m_pSplashScreenSupportItem, "alpha", this);
	m_pSidePanelSupportItemOpacityAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	m_pSidePanelSupportItemOpacityAnimation->setDuration(4000);
	m_pSidePanelSupportItemOpacityAnimation->setStartValue(0);
	m_pSidePanelSupportItemOpacityAnimation->setKeyValueAt(0.25, 1.0);
	m_pSidePanelSupportItemOpacityAnimation->setKeyValueAt(0.75, 1.0);
	m_pSidePanelSupportItemOpacityAnimation->setEndValue(0);

	m_pSidePanelSupportItemAnimationGroup = new QParallelAnimationGroup(this);
	m_pSidePanelSupportItemAnimationGroup->addAnimation(m_pSidePanelSupportItemYAnimation);
	m_pSidePanelSupportItemAnimationGroup->addAnimation(m_pSidePanelSupportItemOpacityAnimation);

	// License Item
	m_pSplashScreenLicenseItem = new SplashScreenItem(this);
	m_pSplashScreenLicenseItem->setUrl(QUrl("http://www.gnu.org/copyleft/gpl.html"));
	m_pSplashScreenLicenseItem->move(450, 45);
	m_pSplashScreenLicenseItem->setPixmap(QPixmap(":/images/splashScreenSidePanelLicense"));

	m_pSidePanelLicenseItemYAnimation = new QPropertyAnimation(m_pSplashScreenLicenseItem, "pos", this);
	m_pSidePanelLicenseItemYAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	m_pSidePanelLicenseItemYAnimation->setDuration(4000);
	m_pSidePanelLicenseItemYAnimation->setStartValue(QPoint(450, 25));
	m_pSidePanelLicenseItemYAnimation->setKeyValueAt(0.25, QPoint(450, 45));
	m_pSidePanelLicenseItemYAnimation->setKeyValueAt(0.75, QPoint(450, 45));
	m_pSidePanelLicenseItemYAnimation->setEndValue(QPoint(450, 65));

	m_pSidePanelLicenseItemOpacityAnimation = new QPropertyAnimation(m_pSplashScreenLicenseItem, "alpha", this);
	m_pSidePanelLicenseItemOpacityAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	m_pSidePanelLicenseItemOpacityAnimation->setDuration(4000);
	m_pSidePanelLicenseItemOpacityAnimation->setStartValue(0);
	m_pSidePanelLicenseItemOpacityAnimation->setKeyValueAt(0.25, 1.0);
	m_pSidePanelLicenseItemOpacityAnimation->setKeyValueAt(0.75, 1.0);
	m_pSidePanelLicenseItemOpacityAnimation->setEndValue(0);

	m_pSidePanelLicenseItemAnimationGroup = new QParallelAnimationGroup(this);
	m_pSidePanelLicenseItemAnimationGroup->addAnimation(m_pSidePanelLicenseItemYAnimation);
	m_pSidePanelLicenseItemAnimationGroup->addAnimation(m_pSidePanelLicenseItemOpacityAnimation);

	m_pSidePanelAnimationGroup = new QSequentialAnimationGroup(this);
	m_pSidePanelAnimationGroup->addAnimation(m_pSidePanelToolItemAnimationGroup);
	m_pSidePanelAnimationGroup->addAnimation(m_pSidePanelSupportItemAnimationGroup);
	m_pSidePanelAnimationGroup->addAnimation(m_pSidePanelLicenseItemAnimationGroup);

	// Items location rect
	m_itemRect.setX(450);
	m_itemRect.setY(25);
	m_itemRect.setWidth(173);
	m_itemRect.setHeight(160);

	setMouseTracking(true);
}

SplashScreen::~SplashScreen()
{
	delete m_pOpacityAnimation;
}

void SplashScreen::show()
{
	QWidget::show();

	m_pOpacityAnimation->start();
}

void SplashScreen::closeWithAnimation()
{
	m_pOpacityAnimation->setStartValue(1.0);
	m_pOpacityAnimation->setEndValue(0.0);
	m_pOpacityAnimation->start();
}

void SplashScreen::onOpacityAnimationFinished()
{
	if(windowOpacity() == 0)
	{
		close();
	}
	else
	{
		m_pSidePanelWidthAnimation->start();
	}
}

void SplashScreen::onSidePanelWidthValueChanged(const QVariant &value)
{
	m_sidePanelWidth = value.toInt();

	repaint();
}

void SplashScreen::onSidePanelWidthAnimationFinished()
{
	m_pSidePanelAnimationGroup->setLoopCount(-1);
	m_pSidePanelAnimationGroup->start();
}

void SplashScreen::mousePressEvent(QMouseEvent *pMouseEvent)
{
	if(pMouseEvent->button() == Qt::LeftButton &&
	   m_itemRect.contains(pMouseEvent->pos()))
	{
		if(m_pSplashScreenToolItem->alpha() != 0.0)
		{
			QDesktopServices::openUrl(m_pSplashScreenToolItem->url());
		}
		else if(m_pSplashScreenSupportItem->alpha() != 0.0)
		{
			QDesktopServices::openUrl(m_pSplashScreenSupportItem->url());
		}
		else if(m_pSplashScreenLicenseItem->alpha() != 0.0)
		{
			QDesktopServices::openUrl(m_pSplashScreenLicenseItem->url());
		}
		else if(m_pSplashScreenAuthorItem->alpha() != 0.0)
		{
			QDesktopServices::openUrl(m_pSplashScreenAuthorItem->url());
		}

		return;
	}

	closeWithAnimation();

	QWidget::mousePressEvent(pMouseEvent);
}

void SplashScreen::paintEvent(QPaintEvent *pPaintEvent)
{
	QPainter painter(this);
	painter.drawPixmap(m_splashScreenPixmap.rect(), m_splashScreenPixmap, m_splashScreenPixmap.rect());

	QBrush brush(m_splasScreenBlured);
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);

	QRect sidePanelRect;
	sidePanelRect.setTopRight(m_splashScreenPixmap.rect().topRight());
	sidePanelRect.setBottomLeft(m_splashScreenPixmap.rect().bottomRight() - QPoint(m_sidePanelWidth, 65));
	painter.drawRect(sidePanelRect);

	QWidget::paintEvent(pPaintEvent);
}

