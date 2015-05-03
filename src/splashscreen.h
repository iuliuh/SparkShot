#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QPixmap>

class QMouseEvent;
class QPaintEvent;
class QPropertyAnimation;
class QVariantAnimation;
class SplashScreenItem;
class QParallelAnimationGroup;
class QSequentialAnimationGroup;

class SplashScreen : public QWidget
{
	Q_OBJECT

public:
	SplashScreen(QWidget *pParent = 0);

	~SplashScreen();

public Q_SLOTS:
	void show();

private Q_SLOTS:
	void closeWithAnimation();
	void onOpacityAnimationFinished();
	void onSidePanelWidthValueChanged(const QVariant& value);
	void onSidePanelWidthAnimationFinished();

protected:
	void mousePressEvent(QMouseEvent *pMouseEvent);
	void paintEvent(QPaintEvent *pPaintEvent);

private:
	QPixmap m_splashScreenPixmap;
	QPixmap m_sidePanelPixmap;
	QPixmap m_splasScreenBlured;

	QPropertyAnimation* m_pOpacityAnimation;

	QVariantAnimation* m_pSidePanelWidthAnimation;

	QPropertyAnimation* m_pSidePanelToolItemYAnimation;
	QPropertyAnimation* m_pSidePanelToolItemOpacityAnimation;
	QPropertyAnimation* m_pSidePanelSupportItemYAnimation;
	QPropertyAnimation* m_pSidePanelSupportItemOpacityAnimation;
	QPropertyAnimation* m_pSidePanelLicenseItemYAnimation;
	QPropertyAnimation* m_pSidePanelLicenseItemOpacityAnimation;
	QPropertyAnimation* m_pSidePanelAuthorItemYAnimation;
	QPropertyAnimation* m_pSidePanelAuthorItemOpacityAnimation;

	QParallelAnimationGroup *m_pSidePanelToolItemAnimationGroup;
	QParallelAnimationGroup *m_pSidePanelSupportItemAnimationGroup;
	QParallelAnimationGroup *m_pSidePanelLicenseItemAnimationGroup;
	QParallelAnimationGroup *m_pSidePanelAuthorItemAnimationGroup;

	QSequentialAnimationGroup *m_pSidePanelAnimationGroup;

	SplashScreenItem* m_pSplashScreenToolItem;
	SplashScreenItem* m_pSplashScreenSupportItem;
	SplashScreenItem* m_pSplashScreenLicenseItem;
	SplashScreenItem* m_pSplashScreenAuthorItem;

	int m_sidePanelWidth;
	QRect m_itemRect;
};

#endif // SPLASHSCREEN_H
