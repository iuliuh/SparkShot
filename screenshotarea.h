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
	void onCurrentToolChanged(const ToolBar::Tool& tool);

private:
	ToolBar *m_pToolBar;

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
