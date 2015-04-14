#ifndef SCREENSHOTAREA_H
#define SCREENSHOTAREA_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QPoint>

class ScreenshotArea : public QWidget
{
	Q_OBJECT

public:
	explicit ScreenshotArea(QWidget *parent = 0);
	~ScreenshotArea();

protected:
	void keyPressEvent(QKeyEvent *pEvent);
	void mouseMoveEvent(QMouseEvent *pEvent);
	void mousePressEvent(QMouseEvent *pEvent);
	void mouseReleaseEvent(QMouseEvent *pEvent);
	void paintEvent(QPaintEvent *pEvent);

public slots:
	void shoot();

private:
	QLabel *m_pDrawingBoard;
	QGridLayout *m_pLayout;

	QPixmap m_originalCapture;

	bool m_leftButtonPressed;
	bool m_selectionStarted;
	QPoint m_initialPressPoint;
	QPoint m_currentPressPoint;

private:
	QColor m_darkOverlayColor;
	QColor m_rubberBandColor;
	int m_rubberBandWidth;
	int m_rubberBandPointRadius;
};

#endif // SCREENSHOTAREA_H
