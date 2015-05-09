#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

// Qt includes
#include <QWidget>
#include <QPixmap>

// Forward declarations
class QMouseEvent;
class QPaintEvent;
class QPropertyAnimation;
class QVariantAnimation;
class SplashScreenItem;
class QParallelAnimationGroup;
class QSequentialAnimationGroup;

//! \brief Class representing the splash screen.
class SplashScreen : public QWidget
{
	Q_OBJECT

public:
	//! \brief Constructs a SplashScreen object.
	//! \brief pParent Parent object.
	SplashScreen(QWidget *pParent = 0);

	//! \brief Destroys a SplashScreen object.
	~SplashScreen();

public Q_SLOTS:
	//! \brief Slot called when the splash screen
	//!        needs to be shown
	void show();

private Q_SLOTS:
	//! \brief Slot called when the splash screen needs to close
	//!        after a fade out animation.
	void closeWithAnimation();

	//! \brief Slot called when the opacity animation finishes.
	void onOpacityAnimationFinished();

	//! \brief Slot called when the side panel width value has changed.
	//! \param value The new side panel width value.
	void onSidePanelWidthValueChanged(const QVariant& value);

	//! \brief Slot called when the side panel width animation has finished.
	void onSidePanelWidthAnimationFinished();

protected:
	//! \see QWidget::mousePressEvent
	void mousePressEvent(QMouseEvent *pMouseEvent);

	//! \see QWidget::paintEvent
	void paintEvent(QPaintEvent *pPaintEvent);

private:
	// Pixmaps constructing the splash screen
	QPixmap m_splashScreenPixmap;
	QPixmap m_sidePanelPixmap;
	QPixmap m_splasScreenBlured;

	// Splash screen opacity animation
	QPropertyAnimation* m_pOpacityAnimation;

	// Splash screen side panel width animation
	QVariantAnimation* m_pSidePanelWidthAnimation;

	// Tools animations
	QPropertyAnimation* m_pSidePanelToolItemYAnimation;
	QPropertyAnimation* m_pSidePanelToolItemOpacityAnimation;
	QPropertyAnimation* m_pSidePanelSupportItemYAnimation;
	QPropertyAnimation* m_pSidePanelSupportItemOpacityAnimation;
	QPropertyAnimation* m_pSidePanelLicenseItemYAnimation;
	QPropertyAnimation* m_pSidePanelLicenseItemOpacityAnimation;
	QPropertyAnimation* m_pSidePanelAuthorItemYAnimation;
	QPropertyAnimation* m_pSidePanelAuthorItemOpacityAnimation;

	// Paralel item animation groups
	QParallelAnimationGroup *m_pSidePanelToolItemAnimationGroup;
	QParallelAnimationGroup *m_pSidePanelSupportItemAnimationGroup;
	QParallelAnimationGroup *m_pSidePanelLicenseItemAnimationGroup;
	QParallelAnimationGroup *m_pSidePanelAuthorItemAnimationGroup;

	// Sequential item animation group
	QSequentialAnimationGroup *m_pSidePanelAnimationGroup;

	// Tool items
	SplashScreenItem* m_pSplashScreenToolItem;
	SplashScreenItem* m_pSplashScreenSupportItem;
	SplashScreenItem* m_pSplashScreenLicenseItem;
	SplashScreenItem* m_pSplashScreenAuthorItem;

	// Panel width
	int m_sidePanelWidth;

	// Item rectangle
	QRect m_itemRect;
};

#endif // SPLASHSCREEN_H
