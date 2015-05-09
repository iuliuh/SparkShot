#ifndef SPLASHSCREENITEM_H
#define SPLASHSCREENITEM_H

// Qt includes
#include <QWidget>
#include <QUrl>

// Forward declaration
class QPaintEvent;

//! \brief Widget representing a side panel item on a SpalshScreen.
class SplashScreenItem : public QWidget
{
	Q_OBJECT

	//! \brief Item alpha value in range 0.0 to 1.0.
	Q_PROPERTY(qreal alpha READ alpha WRITE setAlpha)

public:
	//! \brief Constructs a SplashScreenItem object.
	//! \param pParent Parent object.
	explicit SplashScreenItem(QWidget *pParent = 0);

	//! \brief Destroys a SplashScreenItem object.
	~SplashScreenItem();

	//! \brief Sets the pixmap for the item.
	//! \param pixmap The pixmap to be set.
	void setPixmap(const QPixmap &pixmap);

	//! \brief Provides the item corresponding pixmap.
	//! \return The item pixmap.
	QPixmap pixmap() const;

	//! \brief Sets the alpha value representing the item's opacity.
	//! \param alpha The value for the alpha channel. The value should
	//!        be in the range 0.0 to 1.0, where 0.0 is fully transparent
	//!        and 1.0 is fully opaque.
	void setAlpha(const qreal &alpha);

	//! \brief Provides the alpha value representing the item's opacity.
	//! \return The value for the alpha channel. The value should
	//!         be in the range 0.0 to 1.0, where 0.0 is fully transparent
	//!         and 1.0 is fully opaque.
	qreal alpha() const;

	//! \brief Sets the item url. If the user clicks on an item the default
	//!        browser will open and he will be directed to the set url.
	//!        If no url is set, clicking the item does nothing.
	//! \param url The url to be directed to after clicking the item.
	void setUrl(const QUrl &url);

	//! \brief Retrieves the item url. If the user clicks on an item the default
	//!        browser will open and he will be directed to the set url.
	//!        If no url is set, clicking the item does nothing.
	//! \return The url to be directed to after clicking the item.
	QUrl url() const;

protected:
	//! \see QWidget::paintEvent
	void paintEvent(QPaintEvent *pPaintEvent);

	//! \see QWidget::enterEvent
	void enterEvent(QEvent *pEvent);

	//! \see QWidget::leaveEvent
	void leaveEvent(QEvent *pEvent);

private:
	// Item pixmap
	QPixmap m_pixmap;

	// Item url
	QUrl m_url;

	// Item alpha value [0.0, 1.0]
	qreal m_alpha;
};

#endif // SPLASHSCREENITEM_H
