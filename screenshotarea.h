#ifndef SCREENSHOTAREA_H
#define SCREENSHOTAREA_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QPoint>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "toolbar.h"
#include "uploaddialog.h"

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
	void onOverlayColorChanged(QColor color);
	void onRubberBandColorChanged(QColor color);
	void onRubberBandWidthChanged(int width);
	void onPenWidthChanged(int width);
	void onDotsRadiusChanged(int radius);
	void onFontSizeChanged(int size);

private slots:
	void onUploadButtonPressed();
	void onSaveButtonPressed();

	void replyFinished();
	void onError(QNetworkReply::NetworkError);
	void onSslErrors(QList<QSslError>);

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
	bool m_moveSelectionArea;
	bool m_textPositioned;
	QPoint m_selectionTopLeftPoint;
	QPoint m_selectionBottomRightPoint;

	QPoint m_topLeftPointBeforeSelectionMove;
	QPoint m_bottomRightPointBeforeSelectionMove;
	QPoint m_pressPointBeforeSelectionMove;
	QRect m_screenShotArea;
	QString m_currentText;
	QPoint m_textPoint;

	// Imgur API
	QString m_clientId;
	QNetworkAccessManager m_networkAccessManager;
	QNetworkReply *m_pNetworkReply;
	UploadDialog *m_pUploadDialog;

private:
	QColor m_darkOverlayColor;
	QColor m_rubberBandColor;
	int m_rubberBandWidth;
	int m_penWidth;
	int m_rubberBandPointRadius;
	int m_fontSize;

private:
	float m_arrowHeight;
	float m_arrowBaseWidth;
};

#endif // SCREENSHOTAREA_H
