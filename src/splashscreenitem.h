#ifndef SPLASHSCREENITEM_H
#define SPLASHSCREENITEM_H

#include <QWidget>
#include <QUrl>

class QPaintEvent;

class SplashScreenItem : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(qreal alpha READ alpha WRITE setAlpha)

public:
	explicit SplashScreenItem(QWidget *parent = 0);
	~SplashScreenItem();

	QPixmap pixmap() const;
	void setPixmap(const QPixmap &pixmap);

	qreal alpha() const;
	void setAlpha(const qreal &alpha);

	QUrl url() const;
	void setUrl(const QUrl &url);

protected:
	void paintEvent(QPaintEvent *pPaintEvent);
	void enterEvent(QEvent *pEvent);
	void leaveEvent(QEvent *pEvent);

private:
	QPixmap m_pixmap;
	QUrl m_url;
	qreal m_alpha;
};

#endif // SPLASHSCREENITEM_H
