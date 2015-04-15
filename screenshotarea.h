#ifndef SCREENSHOTAREA_H
#define SCREENSHOTAREA_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QPoint>

#include "toolbar.h"

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

private slots:
	void onSettingsButtonPressed();
	void onUploadButtonPressed();
	void onSaveButtonPressed();

private:
	void drawRubberBand(QPainter* painter);
	void drawCroppedArea(QPainter* painter);
	void drawDarkOverlay(QPainter* painter);
	void drawArrow(QPainter* painter);
	void drawLine(QPainter* painter);
	void drawSquare(QPainter* painter);
	void drawBrush(QPainter* painter);
	void drawEllipse(QPainter* painter);
	void drawText(QPainter* painter);

private:
	ToolBar *m_pToolBar;

	QPixmap m_originalCapture;
	QPixmap m_paintBoard;
	QPixmap m_helperBoard;

	bool m_leftButtonPressed;
	bool m_selectionStarted;
	QPoint m_initialPressPoint;
	QPoint m_currentPressPoint;
	QRect m_screenShotArea;

private:
	QColor m_darkOverlayColor;
	QColor m_rubberBandColor;
	int m_rubberBandWidth;
	int m_penWidth;
	int m_rubberBandPointRadius;

private:
	float m_arrowHeight;
	float m_arrowBaseWidth;
};

#endif // SCREENSHOTAREA_H
